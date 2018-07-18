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
    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerHandler()));
    timer->start(5000);
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
    bool fGood = vchSecret.SetString("xxxxxx");

    if(fGood)
    {
        CKey key = vchSecret.GetKey();
        if(!key.IsValid()) return;
        CPubKey pubkey = key.GetPubKey();
        CKeyID vchAddress = pubkey.GetID();

        LogPrintf("pubkey hexstr : %s",HexStr(pubkey.begin(),pubkey.end()).c_str());

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

}
