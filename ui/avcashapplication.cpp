#include "ui/avcashapplication.h"
#include "init.h"
#include "avcashcore.h"
#include "util.h"
#include "ui/splashscreen.h"
#include "ui/utilitydialog.h"
#include "wallet/wallet.h"
#include "ui/avcashmain.h"


AvCashApplication::AvCashApplication(int &argc, char **argv):
    QApplication(argc, argv),
    coreThread(0),
    optionsModel(0),
    clientModel(0),
    window(0),
    pollShutdownTimer(0),
#ifdef ENABLE_WALLET
    paymentServer(0),
    walletModel(0),
#endif
    returnValue(0)
{
    setQuitOnLastWindowClosed(false);

    // UI per-platform customization
    // This must be done inside the AvCashApplication constructor, or after it, because
    // PlatformStyle::instantiate requires a QApplication
    std::string platformName;
    platformName = gArgs.GetArg("-uiplatform", AvCashMain::DEFAULT_UIPLATFORM);
    platformStyle = PlatformStyle::instantiate(QString::fromStdString(platformName));
    if (!platformStyle) // Fall back to "other" if specified name not found
        platformStyle = PlatformStyle::instantiate("other");
    assert(platformStyle);
}

AvCashApplication::~AvCashApplication()
{
    if(coreThread)
    {
        qDebug() << __func__ << ": Stopping thread";
        Q_EMIT stopThread();
        coreThread->wait();
        qDebug() << __func__ << ": Stopped thread";
    }

    delete window;
    window = 0;
#ifdef ENABLE_WALLET
    delete paymentServer;
    paymentServer = 0;
#endif
    delete optionsModel;
    optionsModel = 0;
    delete platformStyle;
    platformStyle = 0;
}

#ifdef ENABLE_WALLET
void AvCashApplication::createPaymentServer()
{
    paymentServer = new PaymentServer(this);
}
#endif

void AvCashApplication::createOptionsModel(bool resetSettings)
{
    optionsModel = new OptionsModel(nullptr, resetSettings);
}

void AvCashApplication::createWindow(const NetworkStyle *networkStyle)
{
    window = new AvCashMain(platformStyle, networkStyle, 0);

    pollShutdownTimer = new QTimer(window);
    connect(pollShutdownTimer, SIGNAL(timeout()), window, SLOT(detectShutdown()));
}

void AvCashApplication::createSplashScreen(const NetworkStyle *networkStyle)
{
    SplashScreen *splash = new SplashScreen(0, networkStyle);
    // We don't hold a direct pointer to the splash screen after creation, but the splash
    // screen will take care of deleting itself when slotFinish happens.
    splash->show();
    connect(this, SIGNAL(splashFinished(QWidget*)), splash, SLOT(slotFinish(QWidget*)));
    connect(this, SIGNAL(requestedShutdown()), splash, SLOT(close()));
}

void AvCashApplication::startThread()
{
    if(coreThread)
        return;
    coreThread = new QThread(this);
    AvCashCore *executor = new AvCashCore();
    executor->moveToThread(coreThread);

    /*  communication to and from thread */
    connect(executor, SIGNAL(initializeResult(bool)), this, SLOT(initializeResult(bool)));
    connect(executor, SIGNAL(shutdownResult()), this, SLOT(shutdownResult()));
    connect(executor, SIGNAL(runawayException(QString)), this, SLOT(handleRunawayException(QString)));
    connect(this, SIGNAL(requestedInitialize()), executor, SLOT(initialize()));
    connect(this, SIGNAL(requestedShutdown()), executor, SLOT(shutdown()));
    /*  make sure executor object is deleted in its own thread */
    connect(this, SIGNAL(stopThread()), executor, SLOT(deleteLater()));
    connect(this, SIGNAL(stopThread()), coreThread, SLOT(quit()));

    coreThread->start();
}

void AvCashApplication::parameterSetup()
{
    InitLogging();
    InitParameterInteraction();
}

void AvCashApplication::requestInitialize()
{
    qDebug() << __func__ << ": Requesting initialize";
    startThread();
    Q_EMIT requestedInitialize();
}

void AvCashApplication::requestShutdown()
{
    // Show a simple window indicating shutdown status
    // Do this first as some of the steps may take some time below,
    // for example the RPC console may still be executing a command.
    shutdownWindow.reset(ShutdownWindow::showShutdownWindow(window));

    qDebug() << __func__ << ": Requesting shutdown";
    startThread();
    window->hide();
    window->setClientModel(0);
    pollShutdownTimer->stop();

#ifdef ENABLE_WALLET
    window->removeAllWallets();
    delete walletModel;
    walletModel = 0;
#endif
    delete clientModel;
    clientModel = 0;

    StartShutdown();

    // Request shutdown from core thread
    Q_EMIT requestedShutdown();
}

void AvCashApplication::initializeResult(bool success)
{
    qDebug() << __func__ << ": Initialization result: " << success;
    // Set exit result.
    returnValue = success ? EXIT_SUCCESS : EXIT_FAILURE;
    if(success)
    {
        // Log this only after AppInitMain finishes, as then logging setup is guaranteed complete
        qWarning() << "Platform customization:" << platformStyle->getName();
#ifdef ENABLE_WALLET
        PaymentServer::LoadRootCAs();
        paymentServer->setOptionsModel(optionsModel);
#endif

        clientModel = new ClientModel(optionsModel);
        window->setClientModel(clientModel);

#ifdef ENABLE_WALLET
        // TODO: Expose secondary wallets
        if (!vpwallets.empty())
        {
            walletModel = new WalletModel(platformStyle, vpwallets[0], optionsModel);

            window->addWallet(AvCashMain::DEFAULT_WALLET, walletModel);
            window->setCurrentWallet(AvCashMain::DEFAULT_WALLET);

            connect(walletModel, SIGNAL(coinsSent(CWallet*,SendCoinsRecipient,QByteArray)),
                             paymentServer, SLOT(fetchPaymentACK(CWallet*,const SendCoinsRecipient&,QByteArray)));
        }
#endif

        // If -min option passed, start window minimized.
        if(gArgs.GetBoolArg("-min", false))
        {
            window->showMinimized();
        }
        else
        {
            window->show();
        }
        Q_EMIT splashFinished(window);

#ifdef ENABLE_WALLET
        // Now that initialization/startup is done, process any command-line
        // bitcoin: URIs or payment requests:
        connect(paymentServer, SIGNAL(receivedPaymentRequest(SendCoinsRecipient)),
                         window, SLOT(handlePaymentRequest(SendCoinsRecipient)));
        connect(window, SIGNAL(receivedURI(QString)),
                         paymentServer, SLOT(handleURIOrFile(QString)));
        connect(paymentServer, SIGNAL(message(QString,QString,unsigned int)),
                         window, SLOT(message(QString,QString,unsigned int)));
        QTimer::singleShot(100, paymentServer, SLOT(uiReady()));
#endif
        pollShutdownTimer->start(200);
    } else {
        Q_EMIT splashFinished(window); // Make sure splash screen doesn't stick around during shutdown
        quit(); // Exit first main loop invocation
    }
}

void AvCashApplication::shutdownResult()
{
    quit(); // Exit second main loop invocation after shutdown finished
}

void AvCashApplication::handleRunawayException(const QString &message)
{
    QMessageBox::critical(0, "Runaway exception", AvCashMain::tr("A fatal error occurred. Bitcoin can no longer continue safely and will quit.") + QString("\n\n") + message);
    ::exit(EXIT_FAILURE);
}

WId AvCashApplication::getMainWinId() const
{
    if (!window)
        return 0;

    return window->winId();
}






















