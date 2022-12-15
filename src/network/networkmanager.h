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
    QMap<QString, NetworkDevice *> m_devicesByPath;
    QMap<QString, QDBusObjectPath> m_exportMap;
public:
    NetworkManager(QDBusConnection *connection);
public Q_SLOTS:
    void onDeviceAdded(QDBusObjectPath path);
    void onDeviceRemoved(QDBusObjectPath path);
};

#endif // NETWORKMANAGER_H