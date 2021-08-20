#include "ofono.h"
#include "cutie_shell_adaptor.h"

Ofono::Ofono(QDBusConnection *connection)
{
    this->connection = connection;

    this->ofono = new org::ofono::Manager(
        "org.ofono", "/",
        QDBusConnection::systemBus());

    this->modemList = new QList<OfonoModem *>();

    qDBusRegisterMetaType<ofonoPair>();
    qDBusRegisterMetaType<OfonoServices>();

    QDBusPendingReply<OfonoServices> ofonoModems = this->ofono->GetModems();
    connect(this->ofono, SIGNAL(ModemAdded(QDBusObjectPath, QVariantMap)), this, SLOT(onModemAdded(QDBusObjectPath, QVariantMap)));
    ofonoModems.waitForFinished();
    if (!ofonoModems.isError()) {
        for (int i = 0; i < ofonoModems.value().count(); i++) {
            OfonoModem *modem = new OfonoModem(ofonoModems.value().at(i).first.path());
            this->modemList->append(modem);
            new ModemAdaptor(modem);
            connection->registerObject(QString("/modem/").append(QString::number(i)), modem);
            QDBusObjectPath newPath;
            newPath.setPath(QString("/modem/").append(QString::number(i)));
            ModemAdded(newPath);
        }
    }
}

unsigned int Ofono::ModemCount() {
    return modemList->count();
}

void Ofono::onModemAdded(QDBusObjectPath path, QVariantMap properties) {
    OfonoModem *modem = new OfonoModem(path.path());
    this->modemList->append(modem);
    new ModemAdaptor(modem);
    connection->registerObject(QString("/modem/").append(QString::number(modemList->count() - 1)), modem);
    QDBusObjectPath newPath;
    newPath.setPath(QString("/modem/").append(QString::number(modemList->count() - 1)));
    ModemAdded(newPath);
}