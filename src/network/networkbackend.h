#ifndef NETWORK_BACKEND_H
#define NETWORK_BACKEND_H

#include <QDebug>
#include <iostream>
#include <QDBusConnection>
#include "structures.h"
#include "networkdevice.h"

class NetworkDevice;

class NetworkBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QDBusObjectPath> Devices READ devicePaths NOTIFY DevicesChanged)
protected:
    QDBusConnection *m_connection;
    QList<NetworkDevice *> m_devices;
    QList<QDBusObjectPath> m_devicePaths;
public:
    NetworkBackend(QDBusConnection *connection);
    QList<QDBusObjectPath> devicePaths();
    static int next_id;
signals:
    void DevicesChanged(QList<QDBusObjectPath> devicePaths);
};

#endif // NETWORK_BACKEND_H