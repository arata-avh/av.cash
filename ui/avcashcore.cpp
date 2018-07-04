#include "avcashcore.h"


#include "init.h"
#include "rpc/server.h"
#include "ui_interface.h"
#include "util.h"
#include "warnings.h"
#include "wallet/wallet.h"
#include "ui/guiutil.h"

#include <stdint.h>
#include <boost/thread.hpp>

#include <QApplication>
#include <QDebug>


AvCashCore::AvCashCore() :
    QObject()
{

}

void AvCashCore::handleRunawayException(const std::exception *e)
{
    PrintExceptionContinue(e,"Runaway exception");
    Q_EMIT runawayException(QString::fromStdString(GetWarnings("gui")));
}

bool AvCashCore::baseInitialize()
{
    if(!AppInitParameterInteraction())
        return false;

    if(!AppInitParameterInteraction())
        return false;

    if(!AppInitSanityChecks())
        return false;

    if(!AppInitLockDataDirectory())
        return false;

    return true;
}

void AvCashCore::initialize()
{
    try
    {
        qDebug() << __func__ << " : Running initialization in thread";
        bool rv = AppInitMain();
        Q_EMIT initializeResult(rv);
    }
    catch(const std::exception& e){
        handleRunawayException(&e);
    }catch(...){
        handleRunawayException(nullptr);
    }
}

void AvCashCore::shutdown()
{
    try
    {
        qDebug() << __func__ << ": Running Shutdown in thread";
        Interrupt();
        Shutdown();
        qDebug() << __func__ << ": Shutdown finished";
        Q_EMIT shutdownResult();
    } catch (const std::exception& e) {
        handleRunawayException(&e);
    } catch (...) {
        handleRunawayException(nullptr);
    }
}































