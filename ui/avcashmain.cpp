#include "avcashmain.h"
#include "ui_avcashmain.h"
#include "init.h"
#include "util.h"
#include "ui_interface.h"

#include "ui/networkstyle.h"
#include "ui/guiutil.h"
#include "ui/walletmodel.h"
#include "ui/rpcconsole.h"
#include "ui/optionsmodel.h"
#include "ui/clientmodel.h"




const std::string AvCashMain::DEFAULT_UIPLATFORM =
#if defined(Q_OS_MAC)
        "macosx"
#elif defined(Q_OS_WIN)
        "windows"
#else
        "other"
#endif
        ;

/** Display name for default wallet name. Uses tilde to avoid name
 * collisions in the future with additional wallets */
const QString AvCashMain::DEFAULT_WALLET = "~Default";

AvCashMain::AvCashMain(const PlatformStyle *_platformStyle, const NetworkStyle *networkStyle, QWidget *parent) :
    QMainWindow(parent),
    clientModel(0),
    rpcConsole(0),
    platformStyle(_platformStyle),
    ui(new Ui::AvCashMain)
{
    QString windowTitle = tr(PACKAGE_NAME) + " - ";

    #ifdef ENABLE_WALLET
        enableWallet = WalletModel::isWalletEnabled();
    #endif // ENABLE_WALLET

    if(enableWallet)
    {
        windowTitle += tr("Wallet");
    } else {
        windowTitle += tr("Node");
    }
    windowTitle += " " + networkStyle->getTitleAddText();

    setWindowTitle(windowTitle);

    rpcConsole = new RPCConsole(_platformStyle,0);

    this->installEventFilter(this);

    subscribeToCoreSignals();

    ui->setupUi(this);

    pSuperNodeTest = new Form_SuperNode_test(this);

}

AvCashMain::~AvCashMain()
{
    unsubscribeFromCoreSignals();

    delete ui;
}

void AvCashMain::detectShutdown()
{
    if(ShutdownRequested())
    {
        if(rpcConsole)
            rpcConsole->hide();

        qApp->quit();
    }
}

void AvCashMain::setClientModel(ClientModel *_clientModel)
{

    this->clientModel = _clientModel;
    if(_clientModel)
    {
        connect(_clientModel, SIGNAL(numConnectionsChanged(int)), this, SLOT(setNumConnections(int)));
        connect(_clientModel, SIGNAL(networkActiveChanged(bool)), this, SLOT(setNetworkActive(bool)));

        setNumBlocks(_clientModel->getNumBlocks(), _clientModel->getLastBlockDate(), _clientModel->getVerificationProgress(nullptr), false);
        connect(_clientModel, SIGNAL(numBlocksChanged(int,QDateTime,double,bool)), this, SLOT(setNumBlocks(int,QDateTime,double,bool)));

        // Receive and report messages from client model
        connect(_clientModel, SIGNAL(message(QString,QString,unsigned int)), this, SLOT(message(QString,QString,unsigned int)));

        // Show progress dialog
        connect(_clientModel, SIGNAL(showProgress(QString,int)), this, SLOT(showProgress(QString,int)));

        rpcConsole->setClientModel(_clientModel);

        OptionsModel* optionsModel = _clientModel->getOptionsModel();
    }
}

#ifdef ENABLE_WALLET
bool AvCashMain::addWallet(const QString& name, WalletModel *walletModel)
{
    //+++++
    /*
    if(!walletFrame)
        return false;
    setWalletActionsEnabled(true);
    return walletFrame->addWallet(name, walletModel);
    */
    return true;
}

bool AvCashMain::setCurrentWallet(const QString& name)
{
    //+++++
    /*
    if(!walletFrame)
        return false;
    return walletFrame->setCurrentWallet(name);
    */
    return true;
}

void AvCashMain::removeAllWallets()
{
    //+++++
    /*
    if(!walletFrame)
        return;
    setWalletActionsEnabled(false);
    walletFrame->removeAllWallets();
    */
}
#endif // ENABLE_WALLET

void AvCashMain::closeEvent(QCloseEvent *event)
{
#ifndef Q_OS_MAC // Ignored on Mac
    if(clientModel && clientModel->getOptionsModel())
    {
        if(!clientModel->getOptionsModel()->getMinimizeOnClose())
        {
            // close rpcConsole in case it was open to make some space for the shutdown window
            rpcConsole->close();

            QApplication::quit();
        }
        else
        {
            QMainWindow::showMinimized();
            event->ignore();
        }
    }
#else
    QMainWindow::closeEvent(event);

    qApp->quit();
#endif
}

void AvCashMain::setNumConnections(int count)
{
    LogPrintf("peer connect num : %d",count);
}

void AvCashMain::setNetworkActive(bool networkActive)
{

}

void AvCashMain::setNumBlocks(int count, const QDateTime &blockDate, double nVerificationProgress, bool headers)
{
    LogPrintf("Blocks Nums : %d",count);
}

void AvCashMain::message(const QString &title, const QString &message, unsigned int style, bool *ret)
{
    LogPrintf("message : %s - %s",title.toStdString().c_str(),message.toStdString().c_str());
}

#ifdef ENABLE_WALLET
void AvCashMain::setEncryptionStatus(int status)
{

}

void AvCashMain::setHDStatus(int hdEnabled)
{
    LogPrintf("HD Status : %d",hdEnabled);
}

bool AvCashMain::handlePaymentRequest(const SendCoinsRecipient &recipient)
{
    return true;
}

void AvCashMain::incomingTransaction(const QString &date, int unit, const CAmount &amount, const QString &type, const QString &address, const QString &label)
{
    LogPrintf("coming Transaction : %s, num : %lld,from address : %d",date.toStdString().c_str(),amount,address.toStdString().c_str());
}

void AvCashMain::showProgress(const QString &title, int nProgress)
{
    LogPrintf("showProgress : %s - %d ",title.toStdString().c_str(),nProgress);
}

#endif // ENABLE_WALLET



static bool ThreadSafeMessageBox(AvCashMain *gui, const std::string& message, const std::string& caption, unsigned int style)
{
    bool modal = (style & CClientUIInterface::MODAL);
    // The SECURE flag has no effect in the Qt GUI.
    // bool secure = (style & CClientUIInterface::SECURE);
    style &= ~CClientUIInterface::SECURE;
    bool ret = false;
    // In case of modal message, use blocking connection to wait for user to click a button
    QMetaObject::invokeMethod(gui, "message",
                               modal ? GUIUtil::blockingGUIThreadConnection() : Qt::QueuedConnection,
                               Q_ARG(QString, QString::fromStdString(caption)),
                               Q_ARG(QString, QString::fromStdString(message)),
                               Q_ARG(unsigned int, style),
                               Q_ARG(bool*, &ret));
    return ret;
}

void AvCashMain::subscribeToCoreSignals()
{
    // Connect signals to client
    uiInterface.ThreadSafeMessageBox.connect(boost::bind(ThreadSafeMessageBox, this, _1, _2, _3));
    uiInterface.ThreadSafeQuestion.connect(boost::bind(ThreadSafeMessageBox, this, _1, _3, _4));
}

void AvCashMain::unsubscribeFromCoreSignals()
{
    // Disconnect signals from client
    uiInterface.ThreadSafeMessageBox.disconnect(boost::bind(ThreadSafeMessageBox, this, _1, _2, _3));
    uiInterface.ThreadSafeQuestion.disconnect(boost::bind(ThreadSafeMessageBox, this, _1, _3, _4));
}










