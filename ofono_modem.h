#ifndef OFONO_MODEM_H
#define OFONO_MODEM_H

#include <QDebug>
#include <iostream>

#include "ofono_interface.h"
#include "ofono_modem_interface.h"

class OfonoModem : public QObject
{
    Q_OBJECT
private:
    org::ofono::Modem *modem;
    org::ofono::NetworkRegistration *network;
    QString path;
public:
    OfonoModem(QString path);
public Q_SLOTS:
    bool GetOnline();
    void OnlineModem(bool online);

    bool GetPower();
    void PowerModem(bool power);

    QString GetNetName();
    uchar GetNetStrength();

    void onModemPropertyChanged(QString name, QDBusVariant value);
    void onNetworkPropertyChanged(QString name, QDBusVariant value);
Q_SIGNALS:
    void OnlineChanged(bool online);
    void PowerChanged(bool power);
    void NetNameChanged(QString name);
    void NetStrengthChanged(uchar name);
};

#endif // OFONO_MODEM_H