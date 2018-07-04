#include "ui/networkstyle.h"
#include "ui/guiconstants.h"

#include <QApplication>

static const struct{
    const char *networkId;
    const char *appName;
    const int iconColorHurShift;
    const int iconColorSaturationReduction;
    const char *titleAddText;
} network_styles[] = {
    {"main",QAPP_APP_NAME_DEFAULT,0,0,""},
    {"test",QAPP_APP_NAME_TESTNET,70,30,QT_TRANSLATE_NOOP("SplashScreen","[testnet]")},
    {"regtest",QAPP_APP_NAME_TESTNET,160,30,"[regtest]"}
};

static const unsigned network_styles_count = sizeof(network_styles)/sizeof(*network_styles);


NetworkStyle::NetworkStyle(const QString &_appName, const int _iconColorHueShift,
                           const int _iconColorSaturationReduction, const char *_titleAddText):
    appName(_appName),
    titleAddText(qApp->translate("SplashScreen",_titleAddText))
{

}

const NetworkStyle *NetworkStyle::instantiate(const QString &networkId)
{
    for(unsigned x = 0;x < network_styles_count;++x)
    {
        if(networkId == network_styles[x].networkId)
        {
            return new NetworkStyle(
                        network_styles[x].appName,
                        network_styles[x].iconColorHurShift,
                        network_styles[x].iconColorSaturationReduction,
                        network_styles[x].titleAddText);
        }
    }

    return 0;
}
