#include "form_supernode_test.h"
#include "ui_form_supernode_test.h"
#include "validation.h"
#include "chain.h"
#include "chainparams.h"
#include "util.h"
#include "wallet/wallet.h"
#include "script/script.h"
#include "rpc/mining.h"
#include "base58.h"

Form_SuperNode_test::Form_SuperNode_test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_SuperNode_test),
    timeNumber(0)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

Form_SuperNode_test::~Form_SuperNode_test()
{
    delete ui;
}

void Form_SuperNode_test::on_btn_getblockchaininfo_clicked()
{
    LogPrintf("[getblockchaininfo]\n");
    LogPrintf("chain=%s\n",Params().NetworkIDString());
    LogPrintf("blocks=%d\n",(int)chainActive.Height());
    LogPrintf("bestblockhash=%s\n\n",chainActive.Tip()->GetBlockHash().GetHex());
}

void Form_SuperNode_test::on_btn_generate_clicked()
{
    auto_generate();
}

void Form_SuperNode_test::auto_generate()
{
    //LogPrintf("[generate]\n");

    if(::vpwallets.empty()) return;
    if(::vpwallets.size() <= 0) return;

    CWallet* const pwallet = ::vpwallets[0];

    std::shared_ptr<CReserveScript> coinbase_script;
    pwallet->GetScriptForMining(coinbase_script);

    if(!coinbase_script) return;
    if(coinbase_script->reserveScript.empty()) return;

    UniValue univalue = generateBlocks(coinbase_script,1,1000000,true);
}

void Form_SuperNode_test::on_btn_key_test_clicked()
{
    CBitcoinSecret vchSecret;
    bool fGood = vchSecret.SetString("xxx");

    if(fGood)
    {
        CKey key = vchSecret.GetKey();
        if(!key.IsValid()) return;

        std::string strAddr = EncodeDestination(GetDestinationForKey(key.GetPubKey(),OUTPUT_TYPE_P2SH_SEGWIT));

        LogPrintf("strAddr = %s\n",strAddr.c_str());
    }

}

void Form_SuperNode_test::timerHandler()
{
    auto_generate();

    timeNumber += 1;

    ui->lb1->setText("generate : " + QString::number(timeNumber) + " times");
}

void Form_SuperNode_test::on_btn_getBlockByPos_clicked()
{

}

void Form_SuperNode_test::on_btn_unspent_clicked()
{
    if(::vpwallets.empty()) return;
    if(::vpwallets.size() <= 0) return;

    CWallet* const pwallet = ::vpwallets[0];

    int nMinDepth = 1;
    int nMaxDepth = 9999999;

    // Array of avh addresses to filter
    std::set<CTxDestination> destinations;

    CAmount nMinimumAmount = 0;
    CAmount nMaximumAmount = MAX_MONEY;
    uint64_t nMaximumCount = 0;
    CAmount nMinimumSumAmount = MAX_MONEY;

    pwallet->BlockUntilSyncedToCurrentChain();
    std::vector<COutput> vecOutputs;
    LOCK2(cs_main,pwallet->cs_wallet);

    pwallet->AvailableCoins(vecOutputs,true,nullptr,nMinimumAmount,nMaximumAmount,
                                nMinimumSumAmount,nMaximumCount,nMinDepth,nMaxDepth);

    for(const COutput& out : vecOutputs)
    {
        CTxDestination address;
        std::string keyAddr;

        const CScript& scriptPubKey = out.tx->tx->vout[out.i].scriptPubKey;
        bool fValidAddress = ExtractDestination(scriptPubKey,address);

        keyAddr = EncodeDestination(address);


        if(destinations.size() && (!fValidAddress || !destinations.count(address)))
            continue;
    }
}

void Form_SuperNode_test::on_btn_autogenerate_clicked()
{
    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerHandler()));
    timer->start(5100);
}

void Form_SuperNode_test::on_btn_transaction_clicked()
{

}









