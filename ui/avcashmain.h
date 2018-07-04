#ifndef AVCASHMAIN_H
#define AVCASHMAIN_H

#include <QMainWindow>
#include "avcashcore.h"
#include "amount.h"


// SuperNode test form
#include "ui/forms/test/form_supernode_test.h"

class ClientModel;
class NetworkStyle;
class Notificator;
class OptionsModel;
class PlatformStyle;
class RPCConsole;
class SendCoinsRecipient;
class UnitDisplayStatusBarControl;
class WalletFrame;
class WalletModel;
class HelpMessageDialog;
class ModalOverlay;

QT_BEGIN_NAMESPACE
class QAction;
class QProgressBar;
class QProgressDialog;
QT_END_NAMESPACE

#include "config/avcash-config.h"

namespace Ui {
class AvCashMain;
}

class AvCashMain : public QMainWindow
{
    Q_OBJECT

public:
    static const QString DEFAULT_WALLET;
    static const std::string DEFAULT_UIPLATFORM;

    explicit AvCashMain(const PlatformStyle *platformStyle, const NetworkStyle *networkStyle, QWidget *parent = 0);
    ~AvCashMain();

    /* Set the client model.
        The client model represents the part of the core that communicates with the P2P network, and is wallet-agnostic.
    */
    void setClientModel(ClientModel *_clientModel);

#ifdef ENABLE_WALLET
    /*  Set the wallet model.
        The wallet model represents a bitcoin wallet, and offers access to the list of transactions, address book and sending
        functionality.
    */
    bool addWallet(const QString& name, WalletModel *walletModel);
    bool setCurrentWallet(const QString& name);
    void removeAllWallets();
#endif // ENABLE_WALLET
    bool enableWallet;

protected:
    void closeEvent(QCloseEvent *event);

private:
    /** Connect core signals to GUI client */
    void subscribeToCoreSignals();
    /** Disconnect core signals from GUI client */
    void unsubscribeFromCoreSignals();

private:
    Ui::AvCashMain *ui;

    ClientModel             *clientModel;
    RPCConsole              *rpcConsole;
    const PlatformStyle     *platformStyle;
    // Keep track of previous number of blocks,to detect progress
    int prevBlocks;

Q_SIGNALS:
    // Signal raised when a URI was entered or draggedto the GUI
    void receivedURI(const QString &uri);

public Q_SLOTS:
    // Set number of connections shown in the UI
    void setNumConnections(int count);
    // Set network state shown in the UI
    void setNetworkActive(bool networkActive);
    // Set number of blocks and last block date shown in the UI
    void setNumBlocks(int count, const QDateTime& blockDate, double nVerificationProgress, bool headers);

    /* Notify the user of an event from the core network or transaction handling code.
       @param[in] title     the message box / notification title
       @param[in] message   the displayed text
       @param[in] style     modality and style definitions (icon and used buttons - buttons only for message boxes)
                            @see CClientUIInterface::MessageBoxFlags
       @param[in] ret       pointer to a bool that will be modified to whether Ok was clicked (modal only)
    */
    void message(const QString &title, const QString &message, unsigned int style, bool *ret = nullptr);

#ifdef ENABLE_WALLET
    /* Set the encryption status as shown in the UI.
       @param[in] status            current encryption status
       @see WalletModel::EncryptionStatus
    */
    void setEncryptionStatus(int status);

    /* Set the hd-enabled status as shown in the UI.
     @param[in] status            current hd enabled status
     @see WalletModel::EncryptionStatus
     */
    void setHDStatus(int hdEnabled);

    bool handlePaymentRequest(const SendCoinsRecipient& recipient);

    /* Show incoming transaction notification for new transactions. */
    void incomingTransaction(const QString& date, int unit, const CAmount& amount, const QString& type, const QString& address, const QString& label);
#endif // ENABLE_WALLET

private Q_SLOTS:
    // called by a timer to check if fRequestShutdown has been set
    void detectShutdown();
    // Show progress dialog e.g. for verifychain
    void showProgress(const QString &title,int nProgress);

private:
    Form_SuperNode_test* pSuperNodeTest;

};

#endif // AVCASHMAIN_H
