#ifndef NETWORKS_H
#define NETWORKS_H

#include <QDebug>
#include <iostream>
#include "structures.h"
#include "networkmanager.h"

class Networking : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QDBusObjectPath> Backends READ backendPaths NOTIFY BackendsChanged)
private:
    QList<NetworkBackend *> m_backends;
    QList<QDBusObjectPath> m_backendPaths;
    QDBusConnection *connection;
public:
    Networking(QDBusConnection *connection);
    QList<QDBusObjectPath> backendPaths();
Q_SIGNALS:
    void BackendsChanged(QList<QDBusObjectPath> backendPaths);
};

#endif // NETWORK_H