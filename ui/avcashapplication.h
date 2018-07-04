#ifndef AVCASHAPPLICATION_H
#define AVCASHAPPLICATION_H

#include <QObject>
#include <QApplication>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QLibraryInfo>
#include <QLocale>
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>

#include "paymentserver.h"
#include "ui/networkstyle.h"
#include "ui/platformstyle.h"
#include "ui/optionsmodel.h"
#include "ui/clientmodel.h"

class AvCashMain;


class AvCashApplication : public QApplication
{
    Q_OBJECT
    public:
        explicit AvCashApplication(int &argc, char **argv);
        ~AvCashApplication();

    #ifdef ENABLE_WALLET
        // Create payment server
        void createPaymentServer();
    #endif
        // parameter interaction/setup based on rules
        void parameterSetup();
        // Create options model
        void createOptionsModel(bool resetSettings);
        // Create main window
        void createWindow(const NetworkStyle *networkStyle);
        // Create splash screen
        void createSplashScreen(const NetworkStyle *networkStyle);

        // Request core initialization
        void requestInitialize();
        // Request core shutdown
        void requestShutdown();

        // Get process return value
        int getReturnValue() const { return returnValue; }

        // Get window identifier of QMainWindow (BitcoinGUI)
        WId getMainWinId() const;

    public Q_SLOTS:
        void initializeResult(bool success);
        void shutdownResult();
        // Handle runaway exceptions. Shows a message box with the problem and quits the program.
        void handleRunawayException(const QString &message);

    Q_SIGNALS:
        void requestedInitialize();
        void requestedShutdown();
        void stopThread();
        void splashFinished(QWidget *window);

    private:
        QThread *coreThread;
        OptionsModel *optionsModel;
        ClientModel *clientModel;
        AvCashMain *window;
        QTimer *pollShutdownTimer;
    #ifdef ENABLE_WALLET
        PaymentServer* paymentServer;
        WalletModel *walletModel;
    #endif
        int returnValue;
        const PlatformStyle *platformStyle;
        std::unique_ptr<QWidget> shutdownWindow;

        void startThread();
};

#endif // AVCASHAPPLICATION_H
