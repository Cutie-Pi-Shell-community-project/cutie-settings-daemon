#include <QCoreApplication>
#include "backlight.h"
#include "atmosphere.h"
#include "ofono.h"
#include "connman.h"
#include "cutie_shell_adaptor.h"
#include "structures.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QDBusConnection connection = QDBusConnection::systemBus();

    qRegisterMetaType<ServicePair>("ServicePair");
    qDBusRegisterMetaType<ServicePair>();
    qRegisterMetaType<ServiceList>("ServiceList");
    qDBusRegisterMetaType<ServiceList>();

    Backlight *backlight = new Backlight();
    Atmosphere *atmosphere = new Atmosphere();
    Ofono *ofono = new Ofono(&connection);
    Connman *connman = new Connman(&connection);
    
    new BacklightAdaptor(backlight);
    new AtmosphereAdaptor(atmosphere);
    new OfonoAdaptor(ofono);
    new ConnmanAdaptor(connman);

    connection.registerObject("/backlight", backlight);
    connection.registerObject("/atmosphere", atmosphere);
    connection.registerObject("/modem", ofono);
    connection.registerObject("/connection", connman);

    connection.registerService("org.cutie_shell.SettingsDaemon");

    return app.exec();
}
