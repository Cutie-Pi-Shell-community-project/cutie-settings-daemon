#include "ofono.h"

Ofono::Ofono()
{
    this->ofono = new org::ofono::Manager(
        "org.ofono", "/",
        QDBusConnection::systemBus());

    qDBusRegisterMetaType<ofonoPair>();
    qDBusRegisterMetaType<OfonoServices>();

    QDBusPendingReply<OfonoServices> ofonoModems = this->ofono->GetModems();
    ofonoModems.waitForFinished();
    if (!ofonoModems.isError()) {
        for (int i = 0; i < ofonoModems.value().count(); i++) {
            qDebug() << ofonoModems.value().at(i).first.path() << "\n";
        }
    }
}