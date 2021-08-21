#ifndef CONNMAN_H
#define CONNMAN_H

#include <QDebug>
#include <iostream>
#include "structures.h"
#include "connman_interface.h"

class Connman : public QObject
{
    Q_OBJECT
private:
    net::connman::Manager *connman;
    QList<net::connman::Service *> *wifis = 0;
    net::connman::Service *wifi = 0;
    QDBusConnection *connection;
    QDBusObjectPath wifiPath;
    uchar wifiStrength = 0;
    QString wifiName = "";
    void findCurrentConnection();
public:
    Connman(QDBusConnection *connection);
public Q_SLOTS:
    uchar GetWifiStrength();
    QString GetWifiName();
    void onServicesChanged(ServiceList changed, QList<QDBusObjectPath> removed);
    void onWifiPropertyChanged(QString name, QDBusVariant value);
Q_SIGNALS:
    void WifiStrengthChanged(uchar strength);
    void WifiNameChanged(QString name);
};

#endif // CONNMAN_H