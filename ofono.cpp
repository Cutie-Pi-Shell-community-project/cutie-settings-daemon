#include "ofono.h"
#include "cutiepishellcommunityproject_adaptor.h"

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
    ofonoModems.waitForFinished();
    if (!ofonoModems.isError()) {
        for (int i = 0; i < ofonoModems.value().count(); i++) {
            OfonoModem *modem = new OfonoModem(ofonoModems.value().at(i).first.path());
            this->modemList->append(modem);
            new ModemAdaptor(modem);
            connection->registerObject(QString("/com/github/CutiePiShellCommunityProject/modem/").append(QString::number(i)), modem);
        }
    }
}

unsigned int Ofono::ModemCount() {
    return modemList->count();
}