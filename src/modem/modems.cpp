#include "modems.h"
#include "cutie_shell_adaptor.h"

Modems::Modems(QDBusConnection *connection)
{
    this->connection = connection;

    this->ofono = new org::ofono::Manager(
        "org.ofono", "/",
        QDBusConnection::systemBus());

    this->ofonoModemList = new QList<OfonoModem *>();

    QDBusPendingReply<ServiceList> ofonoModems = this->ofono->GetModems();
    connect(this->ofono, SIGNAL(ModemAdded(QDBusObjectPath, QVariantMap)), this, SLOT(onOfonoModemAdded(QDBusObjectPath, QVariantMap)));
    ofonoModems.waitForFinished();
    if (!ofonoModems.isError()) {
        for (int i = 0; i < ofonoModems.value().count(); i++) {
            OfonoModem *modem = new OfonoModem(ofonoModems.value().at(i).first.path());
            this->ofonoModemList->append(modem);
            new ModemAdaptor(modem);
            connection->registerObject(QString("/modem/").append(QString::number(i)), modem);
            QDBusObjectPath newPath;
            newPath.setPath(QString("/modem/").append(QString::number(i)));
            ModemAdded(newPath);
        }
    }
}

unsigned int Modems::ModemCount() {
    return ofonoModemList->count();
}

void Modems::onOfonoModemAdded(QDBusObjectPath path, QVariantMap properties) {
    Q_UNUSED(properties);
    OfonoModem *modem = new OfonoModem(path.path());
    this->ofonoModemList->append(modem);
    new ModemAdaptor(modem);
    connection->registerObject(QString("/modem/").append(QString::number(ofonoModemList->count() - 1)), QString("Modem"), modem);
    QDBusObjectPath newPath;
    newPath.setPath(QString("/modem/").append(QString::number(ofonoModemList->count() - 1)));
    ModemAdded(newPath);
}