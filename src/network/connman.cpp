#include "connman.h"
#include "networks.h"
#include "cutie_shell_adaptor.h"

Connman::Connman(QDBusConnection *connection)
	: connection(connection)
{
    this->connman = new net::connman::Manager(
        "net.connman", "/",
        QDBusConnection::systemBus());

    this->wifis = new QList<net::connman::Service *>();

    connect(this->connman, SIGNAL(ServicesChanged(ServiceList, QList<QDBusObjectPath>)), this, SLOT(onServicesChanged(ServiceList, QList<QDBusObjectPath>)));

    findCurrentConnection();
}

void Connman::onServicesChanged(ServiceList changed, QList<QDBusObjectPath> removed) {
    Q_UNUSED(changed)
    for (int i = 0; i < removed.count(); i++) {
        if (removed.at(i).path() == wifiPath.path()) {
            findCurrentConnection();
            return;
        }
    }

    if (wifi == 0) {
        findCurrentConnection();
    }
}

void Connman::onWifiPropertyChanged(QString name, QDBusVariant value) {
    net::connman::Service *thisService = (net::connman::Service *)sender();
    if (thisService != wifi) {
        if (wifi == 0) {
            if (name == "State") {
                if (value.variant().value<QString>() == "online" || value.variant().value<QString>() == "ready") {
                    findCurrentConnection();
                }
            }
        }
        return;
    }

    if (name == "Strength") {
        wifiStrength = value.variant().value<uchar>();
        WifiStrengthChanged(wifiStrength);
    }

    if (name == "Name") {
        wifiName = value.variant().value<QString>();
        WifiNameChanged(wifiName);
    }

    if (name == "State") {
        if (value.variant().value<QString>() != "online" && value.variant().value<QString>() != "ready") {
            findCurrentConnection();
        }
    }
}

void Connman::findCurrentConnection() {
    wifiPath.setPath("/");
    wifiStrength = 0;
    if (wifis != 0) {
        for (int i = 0; i < wifis->count(); i++) {
            disconnect(wifis->at(i), SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(onWifiPropertyChanged(QString, QDBusVariant)));
            delete wifis->at(i);
            wifis->removeAt(i);
        }
    }
    wifi = 0;

    QDBusPendingReply<ServiceList> connmanServices = this->connman->GetServices();
    connmanServices.waitForFinished();
    if (!connmanServices.isError()) {
        for (int i = 0; i < connmanServices.value().count(); i++) {
            if (connmanServices.value().at(i).second.value("Type").toString() == "wifi"){
                net::connman::Service *wifi1 = new net::connman::Service("net.connman", 
                    connmanServices.value().at(i).first.path(), QDBusConnection::systemBus());
                connect(wifi1, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(onWifiPropertyChanged(QString, QDBusVariant)));
                wifis->append(wifi1);

                if (connmanServices.value().at(i).second.value("State").toString() == "ready" || 
                    connmanServices.value().at(i).second.value("State").toString() == "online") {
                    
                    wifi = wifi1;
                    wifiPath = connmanServices.value().at(i).first;
                    wifiStrength = connmanServices.value().at(i).second.value("Strength").value<uchar>();
                    wifiName = connmanServices.value().at(i).second.value("Name").value<QString>();
                }
            }
        }
    }

    WifiNameChanged(wifiName);
    WifiStrengthChanged(wifiStrength);
}

uchar Connman::getWifiStrength() {
    return wifiStrength;
}

QString Connman::getWifiName() {
    return wifiName;
}