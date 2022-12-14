#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QDebug>
#include <iostream>
#include <QDBusInterface>
#include "structures.h"
#include "networkbackend.h"
#include "nmwlandevice.h"

class Network;

class NetworkManager : public NetworkBackend
{
    Q_OBJECT
private:
    QDBusInterface nm;
    QList<QDBusObjectPath> wifiDevs;
    QList<QDBusObjectPath> activeWifis;
public:
    NetworkManager(QDBusConnection *connection);
public Q_SLOTS:
    void onDeviceAdded(QDBusObjectPath path);
};

#endif // NETWORKMANAGER_H