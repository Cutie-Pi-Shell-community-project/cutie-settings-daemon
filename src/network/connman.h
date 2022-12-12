#ifndef CONNMAN_H
#define CONNMAN_H

#include <QDebug>
#include <iostream>
#include "structures.h"
#include "connman_interface.h"

class Network;

class Connman : public QObject
{
    Q_OBJECT
private:
    net::connman::Manager *connman;
    QList<net::connman::Service *> *wifis = 0;
    net::connman::Service *wifi = 0;
    QDBusConnection *connection;
    QDBusObjectPath wifiPath;
    uchar wifiStrength;
    QString wifiName;
    void findCurrentConnection();
    Network *parent;
public:
    Connman(QDBusConnection *connection);
    uchar getWifiStrength();
    QString getWifiName();
public Q_SLOTS:
    void onServicesChanged(ServiceList changed, QList<QDBusObjectPath> removed);
    void onWifiPropertyChanged(QString name, QDBusVariant value);
Q_SIGNALS:
    void WifiStrengthChanged(uchar strength);
    void WifiNameChanged(QString name);
};

#endif // CONNMAN_H