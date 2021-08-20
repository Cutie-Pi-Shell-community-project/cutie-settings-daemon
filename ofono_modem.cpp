#include "ofono.h"

OfonoModem::OfonoModem(QString path) {
    this->modem = new org::ofono::Modem("org.ofono", 
        path, QDBusConnection::systemBus());
    this->path = path;

    this->network = 0;

    QDBusPendingReply<QVariantMap> netProperties = modem->GetProperties();
    netProperties.waitForFinished();
    if (!netProperties.isError()) {    
        QList<QVariant> ifaceList = netProperties.value().value("Interfaces").toList();
        for (int i = 0; i < ifaceList.count(); i++) {
            QString ifaceName = ifaceList.at(i).toString();
            if (ifaceName == "org.ofono.NetworkRegistration") {
                this->network = new org::ofono::NetworkRegistration("org.ofono", 
                    path, QDBusConnection::systemBus());
                connect(this->network, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(onNetworkPropertyChanged(QString, QDBusVariant)));
            }
        }
    }
    
    connect(this->modem, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(onModemPropertyChanged(QString, QDBusVariant)));
}

bool OfonoModem::GetOnline() {
    QDBusPendingReply<QVariantMap> netProperties = modem->GetProperties();
    netProperties.waitForFinished();
    if (!netProperties.isError()) {    
        return netProperties.value().value("Online").toBool();
    }
    return false;
}

void OfonoModem::OnlineModem(bool online){
    modem->SetProperty("Online", QDBusVariant(online));
}

bool OfonoModem::GetPower() {
    QDBusPendingReply<QVariantMap> netProperties = modem->GetProperties();
    netProperties.waitForFinished();
    if (!netProperties.isError()) {    
        return netProperties.value().value("Powered").toBool();
    }
    return false;
}

void OfonoModem::PowerModem(bool power){
    modem->SetProperty("Powered", QDBusVariant(power));
}

QString OfonoModem::GetNetName() {
    if (network == 0) return "";
    QDBusPendingReply<QVariantMap> netProperties = network->GetProperties();
    netProperties.waitForFinished();
    if (!netProperties.isError()) {    
        return netProperties.value().value("Name").toString();
    }
    return "";
}

void OfonoModem::onModemPropertyChanged(QString name, QDBusVariant value) {
    if (name == "Online") {
        OnlineChanged(value.variant().toBool());
    }

    if (name == "Powered") {
        PowerChanged(value.variant().toBool());
    }

    if (name == "Interfaces") {
        if (this->network == 0) {
            QList<QVariant> ifaceList = value.variant().toList();
            for (int i = 0; i < ifaceList.count(); i++) {
                QString ifaceName = ifaceList.at(i).toString();
                if (ifaceName == "org.ofono.NetworkRegistration") {
                    this->network = new org::ofono::NetworkRegistration("org.ofono", 
                        path, QDBusConnection::systemBus());
                    connect(this->network, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(onNetworkPropertyChanged(QString, QDBusVariant)));
                    QDBusPendingReply<QVariantMap> netProperties = network->GetProperties();
                    netProperties.waitForFinished();
                    if (!netProperties.isError()) {    
                        NetNameChanged(netProperties.value().value("Name").toString());
                    }
                }
            }
        }
    }
}

void OfonoModem::onNetworkPropertyChanged(QString name, QDBusVariant value) {
    if (name == "Name") {
        NetNameChanged(value.variant().toString());
    }
}