#include "form_supernode_test.h"

#include "ui_form_supernode_test.h"
#include "validation.h"
#include "txmempool.h"
#include "chain.h"
#include "chainparams.h"
#include "util.h"
#include "wallet/wallet.h"
#include "script/script.h"
#include "rpc/mining.h"
#include "base58.h"
#include "core_io.h"
#include "policy/policy.h"

Form_SuperNode_test::Form_SuperNode_test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_SuperNode_test),
    timeNumber(0)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose,true);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerHandler()));
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

    if(timeNumber >= 100)
    {
        timer->stop();
    }
}

void Form_SuperNode_test::on_btn_getBlockByPos_clicked()
{

}

void Form_SuperNode_test::on_btn_unspent_clicked()
{
    if(::vpwallets.empty()) return;
    if(::vpwallets.size() <= 0) return;

    CWallet* const pwallet = ::vpwallets[0];

    vecOutputs.clear();

    int nMinDepth = 1;
    int nMaxDepth = 9999999;

    // Array of avh addresses to filter
    std::set<CTxDestination> destinations;

    CAmount nMinimumAmount = 0;
    CAmount nMaximumAmount = MAX_MONEY;
    uint64_t nMaximumCount = 0;
    CAmount nMinimumSumAmount = MAX_MONEY;

    pwallet->BlockUntilSyncedToCurrentChain();

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
    timeNumber = 0;
    timer->start(100);
}

void Form_SuperNode_test::on_btn_transaction_clicked()
{
    if(::vpwallets.empty()) return;
    if(::vpwallets.size() <= 0) return;

    CWallet* const pwallet = ::vpwallets[0];

    CMutableTransaction rawTx;
    rawTx.nLockTime = 0;

    uint256 txid;
    txid.SetHex("b634f57fbc30ff05eccf94b6cad4bd7fbeacea9227d6b934898d86921bd7c70a");
    int vout = 0;

    // push_back txin,rawTx.vin is a vector
    CTxIn in(COutPoint(txid,vout),CScript(),std::numeric_limits<uint32_t>::max());

    rawTx.vin.push_back(in);

    // push_back txout
    CTxDestination destinations[2];
    CAmount nAmount[2] = {100000000,4895000000};

    destinations[0] = DecodeDestination("2MuUsbP7TjyPj8Sd8pfCkEQzrqCgVn1E7Uw");
    destinations[1] = DecodeDestination("2MuZ7nDqTYND4LovQ5HPiibMp2ciVFihMTv");

    for(int i = 0;i < 2;++i)
    {
        if(!IsValidDestination(destinations[i])) return;
        CScript scriptPubKey = GetScriptForDestination(destinations[i]);
        CTxOut out(nAmount[i],scriptPubKey);
        rawTx.vout.push_back(out);
    }

    std::string testHex = "02000000010ac7d71b92868d8934b9d62792eaacbe7fbdd4cab694cfec05ff30bc7ff534b60000000000ffffffff0200e1f5050000000017a91418850cdddefdc41f63ec56406c6fd4353e3438cb87c0c5c3230100000017a91419528e86163f62b7983decce5e83c37529eda55f8700000000";

    std::string trancationHex = EncodeHexTx(rawTx);

    assert(testHex == trancationHex);

    // signrawtransaction
    LOCK2(cs_main,pwallet ? &pwallet->cs_wallet : nullptr);

    // Fetch previous transactions (inputs):
    CCoinsView viewDummy;
    CCoinsViewCache view(&viewDummy);
    {
        LOCK(mempool.cs);
        CCoinsViewCache &viewChain = *pcoinsTip;
        CCoinsViewMemPool viewMempool(&viewChain,mempool);
        view.SetBackend(viewMempool);

        for(const CTxIn& txin : rawTx.vin)
        {
            view.AccessCoin(txin.prevout);      // Load entries from viewChain into view;can fail
        }

        view.SetBackend(viewDummy);
    }

    if(pwallet)
    {
        EnsureWalletIsUnlocked(pwallet);
    }

    const CKeyStore& keystore = *pwallet;

    int nHashType = SIGHASH_ALL;

    bool fHashSingle = ((nHashType & ~SIGHASH_ANYONECANPAY) == SIGHASH_SINGLE);

    // Script verification errors
    UniValue vErrors(UniValue::VARR);

    // Use CTransaction for the constant parts of the transaction to avoid rehashing.
    const CTransaction txConst(rawTx);
    // Sign what we can:
    for(unsigned int i = 0;i < rawTx.vin.size();i++)
    {
        CTxIn& txin = rawTx.vin[i];
        const Coin& coin = view.AccessCoin(txin.prevout);
        if(coin.IsSpent())
        {
            continue;
        }
        const CScript& prevPubKey = coin.out.scriptPubKey;
        const CAmount& amount = coin.out.nValue;

        SignatureData sigdata;
        // Only sign SIGHASH_SINGLE if there's a corresponding output:
        if(!fHashSingle || (i < rawTx.vout.size()))
            ProduceSignature(MutableTransactionSignatureCreator(&keystore,&rawTx,i,amount,nHashType),prevPubKey,sigdata);
        sigdata = CombineSignatures(prevPubKey,TransactionSignatureChecker(&txConst,i,amount),sigdata,DataFromTransaction(rawTx,i));

        UpdateTransaction(rawTx,i,sigdata);

        ScriptError serror = SCRIPT_ERR_OK;
        if(!VerifyScript(txin.scriptSig,prevPubKey,&txin.scriptWitness,STANDARD_SCRIPT_VERIFY_FLAGS,TransactionSignatureChecker(&txConst,i,amount),&serror))
        {
            if(serror == SCRIPT_ERR_INVALID_ALTSTACK_OPERATION)
            {
                // Unable to sign input and verification failed (possible attemp to partially sign).

            }
            else
            {

            }
        }
    }

    bool fComplete = vErrors.empty();

    trancationHex = EncodeHexTx(rawTx);
    testHex = "02000000010ac7d71b92868d8934b9d62792eaacbe7fbdd4cab694cfec05ff30bc7ff534b60000000048473044022068a06f34179bce94860ccc0b2db72ab47fa8ec803e6893f72c4ee18cbf1fb08702201c8dc95d48c2901acb5c05f4b06b24a4792ab78c2c760d73818cb577c9abc33201ffffffff0200e1f5050000000017a91418850cdddefdc41f63ec56406c6fd4353e3438cb87c0c5c3230100000017a91419528e86163f62b7983decce5e83c37529eda55f8700000000";

    assert(trancationHex == testHex);

    int n = 1;

}









