#ifndef NETWORKSTYLE_H
#define NETWORKSTYLE_H

#include <QPixmap>
#include <QIcon>
#include <QString>

// Coin network-specific GUI style information
class NetworkStyle
{
public:
    // Get style associated with provided BIP70 network id,or 0 if not know
    static const NetworkStyle *instantiate(const QString &networkId);

    const QString &getAppName() const { return appName; }
    const QIcon &getAppIcon() const { return appIcon; }
    const QIcon &getTrayAndWindowIcon() const { return trayAndWindowIcon; }
    const QString &getTitleAddText() const { return titleAddText; }

private:
    NetworkStyle(const QString &_appName,const int _iconColorHueShift,
                 const int _iconColorSaturationReduction,const char *_titleAddText);

    QString appName;
    QIcon appIcon;
    QIcon trayAndWindowIcon;
    QString titleAddText;
};

#endif // NETWORKSTYLE_H
