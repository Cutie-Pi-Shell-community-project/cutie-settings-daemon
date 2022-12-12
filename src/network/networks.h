#ifndef NETWORKS_H
#define NETWORKS_H

#include <QDebug>
#include <iostream>
#include "structures.h"
#include "connman.h"

class Networks : public QObject
{
    Q_OBJECT
private:
    Connman *connman;
    QDBusConnection *connection;
public:
    Networks(QDBusConnection *connection);
public Q_SLOTS:
    uchar GetWifiStrength();
    QString GetWifiName();
Q_SIGNALS:
    void WifiStrengthChanged(uchar strength);
    void WifiNameChanged(QString name);
};

#endif // NETWORK_H