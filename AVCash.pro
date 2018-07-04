#-------------------------------------------------
#
# Project created by QtCreator 2018-06-17T22:46:51
#
#-------------------------------------------------
QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = AVCash
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH +=  /usr/local/include \
                /usr/local/opt/openssl/include \
                /usr/local/opt/berkeley-db@4/include \
                /usr/local/opt/leveldb/include

LIBS += -L/usr/local/lib \
        -L/usr/local/opt/berkeley-db@4/lib/ \
        -L/usr/local/opt/openssl/lib \
        -L/usr/local/opt/leveldb/lib \
        -lboost_system \
        -lboost_filesystem \
        -lboost_thread-mt \
        -lboost_program_options \
        -lboost_chrono \
        -lcrypto \
        -ldl \
        -lstdc++ \
        -lsecp256k1 \
        -lleveldb \
        -lleveldb_sse42 \
        -lmemenv \
        -lminiupnpc \
        -lunivalue \
        -levent \
        -levent_pthreads \
        -ldb \
        -ldb_cxx \
        -lprotobuf \
        -framework Foundation \
        -framework ApplicationServices \
        -framework AppKit



SOURCES += \
        main.cpp \
    ui/avcashmain.cpp \
    addrdb.cpp \
    addrman.cpp \
    arith_uint256.cpp \
    base58.cpp \
    bech32.cpp \
    blockencodings.cpp \
    bloom.cpp \
    chain.cpp \
    chainparams.cpp \
    chainparamsbase.cpp \
    checkpoints.cpp \
    clientversion.cpp \
    coins.cpp \
    compressor.cpp \
    core_read.cpp \
    core_write.cpp \
    dbwrapper.cpp \
    fs.cpp \
    hash.cpp \
    httprpc.cpp \
    httpserver.cpp \
    init.cpp \
    key.cpp \
    keystore.cpp \
    merkleblock.cpp \
    miner.cpp \
    net_processing.cpp \
    net.cpp \
    netaddress.cpp \
    netbase.cpp \
    noui.cpp \
    pow.cpp \
    protocol.cpp \
    pubkey.cpp \
    random.cpp \
    rest.cpp \
    scheduler.cpp \
    sync.cpp \
    threadinterrupt.cpp \
    timedata.cpp \
    torcontrol.cpp \
    txdb.cpp \
    txmempool.cpp \
    ui_interface.cpp \
    uint256.cpp \
    util.cpp \
    utilmoneystr.cpp \
    utilstrencodings.cpp \
    utiltime.cpp \
    validation.cpp \
    validationinterface.cpp \
    versionbits.cpp \
    warnings.cpp \
    compat/strnlen.cpp \
    consensus/merkle.cpp \
    consensus/tx_verify.cpp \
    crypto/aes.cpp \
    crypto/chacha20.cpp \
    crypto/hmac_sha256.cpp \
    crypto/hmac_sha512.cpp \
    crypto/ripemd160.cpp \
    crypto/sha1.cpp \
    crypto/sha256_sse4.cpp \
    crypto/sha256.cpp \
    crypto/sha512.cpp \
    crypto/ctaes/bench.c \
    crypto/ctaes/ctaes.c \
    policy/feerate.cpp \
    policy/fees.cpp \
    policy/policy.cpp \
    policy/rbf.cpp \
    primitives/block.cpp \
    primitives/transaction.cpp \
    rpc/blockchain.cpp \
    rpc/client.cpp \
    rpc/mining.cpp \
    rpc/misc.cpp \
    rpc/rawtransaction.cpp \
    rpc/safemode.cpp \
    rpc/server.cpp \
    script/bitcoinconsensus.cpp \
    script/interpreter.cpp \
    script/ismine.cpp \
    script/script_error.cpp \
    script/script.cpp \
    script/sigcache.cpp \
    script/sign.cpp \
    script/standard.cpp \
    support/cleanse.cpp \
    support/lockedpool.cpp \
    wallet/crypter.cpp \
    wallet/db.cpp \
    wallet/feebumper.cpp \
    wallet/rpcdump.cpp \
    wallet/rpcwallet.cpp \
    wallet/wallet.cpp \
    wallet/walletdb.cpp \
    wallet/walletutil.cpp \
    rpc/rpcprotocol.cpp \
    rpc/rpcnet.cpp \
    rpc/rpcutil.cpp \
    wallet/walletfees.cpp \
    wallet/walletinit.cpp \
    compat/glibc_sanity.cpp \
    compat/glibcxx_sanity.cpp \
    ui/avcashcore.cpp \
    ui/avcashapplication.cpp \
    ui/networkstyle.cpp \
    ui/guiutil.cpp \
    ui/bitcoinunits.cpp \
    ui/walletmodel.cpp \
    ui/paymentrequestplus.cpp \
    ui/walletmodeltransaction.cpp \
    ui/addresstablemodel.cpp \
    ui/optionsmodel.cpp \
    ui/paymentserver.cpp \
    ui/recentrequeststablemodel.cpp \
    ui/transactiontablemodel.cpp \
    ui/platformstyle.cpp \
    ui/transactiondesc.cpp \
    ui/transactionrecord.cpp \
    ui/paymentrequest.pb.cc \
    ui/intro.cpp \
    ui/splashscreen.cpp \
    ui/clientmodel.cpp \
    ui/bantablemodel.cpp \
    ui/peertablemodel.cpp \
    ui/utilitydialog.cpp \
    ui/rpcconsole.cpp \
    ui/trafficgraphwidget.cpp \
    maind.cpp
HEADERS += \
    ui/avcashmain.h \
    addrdb.h \
    addrman.h \
    amount.h \
    arith_uint256.h \
    base58.h \
    bech32.h \
    blockencodings.h \
    bloom.h \
    chain.h \
    chainparams.h \
    chainparamsbase.h \
    chainparamsseeds.h \
    checkpoints.h \
    checkqueue.h \
    clientversion.h \
    coins.h \
    compat.h \
    compressor.h \
    core_io.h \
    core_memusage.h \
    cuckoocache.h \
    dbwrapper.h \
    fs.h \
    hash.h \
    httprpc.h \
    httpserver.h \
    indirectmap.h \
    init.h \
    key.h \
    keystore.h \
    limitedmap.h \
    memusage.h \
    merkleblock.h \
    miner.h \
    net_processing.h \
    net.h \
    netaddress.h \
    netbase.h \
    netmessagemaker.h \
    noui.h \
    pow.h \
    prevector.h \
    protocol.h \
    pubkey.h \
    random.h \
    reverse_iterator.h \
    reverselock.h \
    scheduler.h \
    serialize.h \
    streams.h \
    sync.h \
    threadinterrupt.h \
    threadsafety.h \
    timedata.h \
    tinyformat.h \
    torcontrol.h \
    txdb.h \
    txmempool.h \
    ui_interface.h \
    uint256.h \
    undo.h \
    util.h \
    utilmoneystr.h \
    utilstrencodings.h \
    utiltime.h \
    validation.h \
    validationinterface.h \
    version.h \
    versionbits.h \
    warnings.h \
    compat/byteswap.h \
    compat/endian.h \
    consensus/consensus.h \
    consensus/merkle.h \
    consensus/params.h \
    consensus/tx_verify.h \
    consensus/validation.h \
    crypto/aes.h \
    crypto/chacha20.h \
    crypto/common.h \
    crypto/hmac_sha256.h \
    crypto/hmac_sha512.h \
    crypto/ripemd160.h \
    crypto/sha1.h \
    crypto/sha256.h \
    crypto/sha512.h \
    crypto/ctaes/ctaes.h \
    policy/feerate.h \
    policy/fees.h \
    policy/policy.h \
    policy/rbf.h \
    primitives/block.h \
    primitives/transaction.h \
    rpc/blockchain.h \
    rpc/client.h \
    rpc/mining.h \
    rpc/register.h \
    rpc/safemode.h \
    rpc/server.h \
    script/bitcoinconsensus.h \
    script/interpreter.h \
    script/ismine.h \
    script/script_error.h \
    script/script.h \
    script/sigcache.h \
    script/sign.h \
    script/standard.h \
    support/cleanse.h \
    support/events.h \
    support/lockedpool.h \
    support/allocators/secure.h \
    support/allocators/zeroafterfree.h \
    wallet/coincontrol.h \
    wallet/crypter.h \
    wallet/db.h \
    wallet/feebumper.h \
    wallet/rpcwallet.h \
    wallet/wallet.h \
    wallet/walletdb.h \
    wallet/walletutil.h \
    rpc/rpcprotocol.h \
    rpc/rpcutil.h \
    wallet/walletfees.h \
    wallet/walletinit.h \
    compat/sanity.h \
    ui/avcashcore.h \
    ui/avcashapplication.h \
    ui/guiconstants.h \
    ui/networkstyle.h \
    config/avcash-config.h \
    ui/guiutil.h \
    ui/bitcoinunits.h \
    ui/walletmodel.h \
    ui/paymentrequestplus.h \
    ui/walletmodeltransaction.h \
    ui/addresstablemodel.h \
    ui/optionsmodel.h \
    ui/paymentserver.h \
    ui/recentrequeststablemodel.h \
    ui/transactiontablemodel.h \
    ui/platformstyle.h \
    ui/transactiondesc.h \
    ui/transactionrecord.h \
    ui/paymentrequest.pb.h \
    ui/intro.h \
    ui/splashscreen.h \
    ui/clientmodel.h \
    ui/bantablemodel.h \
    ui/peertablemodel.h \
    ui/utilitydialog.h \
    ui/rpcconsole.h \
    ui/trafficgraphwidget.h
FORMS += \
        ui/forms/avcashmain.ui \
        ui/forms/intro.ui \
    ui/forms/helpmessagedialog.ui \
    ui/forms/debugwindow.ui
DISTFILES += \
    crypto/ctaes/README.md \
    crypto/ctaes/COPYING

RESOURCES += \
    ui/avcash.qrc \
    ui/avcash_locale.qrc
