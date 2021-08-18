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
                connect(this->network, SIGNAL(PropertyChanged(QString, QVariant)), this, SLOT(onNetworkPropertyChanged(QString, QVariant)));
            }
        }
    }
    
    connect(this->modem, SIGNAL(PropertyChanged(QString, QVariant)), this, SLOT(onModemPropertyChanged(QString, QVariant)));
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
    QDBusPendingReply<QVariantMap> netProperties = network->GetProperties();
    netProperties.waitForFinished();
    if (!netProperties.isError()) {    
        return netProperties.value().value("Name").toString();
    }
    return "";
}

void OfonoModem::onModemPropertyChanged(QString name, QVariant value) {
    if (name == "Online") {
        OnlineChanged(value.toBool());
    }

    if (name == "Powered") {
        PowerChanged(value.toBool());
    }

    if (name == "Interfaces") {
        if (this->network == 0) {
            QList<QVariant> ifaceList = value.toList();
            for (int i = 0; i < ifaceList.count(); i++) {
                QString ifaceName = ifaceList.at(i).toString();
                if (ifaceName == "org.ofono.NetworkRegistration") {
                    this->network = new org::ofono::NetworkRegistration("org.ofono", 
                        path, QDBusConnection::systemBus());
                    connect(this->network, SIGNAL(PropertyChanged(QString, QVariant)), this, SLOT(onNetworkPropertyChanged(QString, QVariant)));
                }
            }
        }
    }
}

void OfonoModem::onNetworkPropertyChanged(QString name, QVariant value) {
    if (name == "Name") {
        NetNameChanged(value.toString());
    }
}