#include "ofono.h"

Ofono::Ofono()
{
    this->ofono = new org::ofono::Manager(
        "org.ofono", "/",
        QDBusConnection::systemBus());

    this->ofonoModems = new QList<org::ofono::Modem *>();
    this->ofonoNetworks = new QList<org::ofono::NetworkRegistration *>();

    qDBusRegisterMetaType<ofonoPair>();
    qDBusRegisterMetaType<OfonoServices>();

    QDBusPendingReply<OfonoServices> ofonoModems = this->ofono->GetModems();
    ofonoModems.waitForFinished();
    if (!ofonoModems.isError()) {
        for (int i = 0; i < ofonoModems.value().count(); i++) {
            qDebug() << ofonoModems.value().at(i).first.path() << "\n";
            org::ofono::Modem *modem = new org::ofono::Modem("org.ofono", 
                ofonoModems.value().at(i).first.path(), QDBusConnection::systemBus());
            org::ofono::NetworkRegistration *network = new org::ofono::NetworkRegistration("org.ofono", 
                ofonoModems.value().at(i).first.path(), QDBusConnection::systemBus());
            QDBusPendingReply<QVariantMap> netProperties = network->GetProperties();
            netProperties.waitForFinished();
            if (!netProperties.isError()) {    
                qDebug() << netProperties.value().value("Name").toString() << "\n";
            }
            this->ofonoModems->append(modem);
            this->ofonoNetworks->append(network);
        }
    }
}