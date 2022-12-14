#ifndef NETWORK_DEVICE_H
#define NETWORK_DEVICE_H

#include <QDebug>
#include <iostream>
#include <QDBusConnection>
#include "structures.h"
#include "networkconnection.h"

class NetworkConnection;

class NetworkDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QDBusObjectPath> Services READ apPaths NOTIFY ServicesChanged)
    Q_PROPERTY(QDBusObjectPath ActiveService READ activeServicePath NOTIFY ActiveServiceChanged)
protected:
    QDBusConnection *m_connection;
    QList<NetworkConnection *> m_services;
    NetworkConnection *m_active_service;
    QList<QDBusObjectPath> m_apPaths;
    QDBusObjectPath m_active_service_path;
public:
    NetworkDevice(QDBusConnection *connection);
    NetworkConnection *getActiveService();
    QList<QDBusObjectPath> apPaths();
    QDBusObjectPath activeServicePath();
    static int next_id;
signals:
    void ServicesChanged(QList<QDBusObjectPath> apPaths);
    void ActiveServiceChanged(QDBusObjectPath activeServicePath);
};

#endif // NETWORK_DEVICE_H