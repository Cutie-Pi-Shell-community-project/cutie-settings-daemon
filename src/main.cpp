#include <QCoreApplication>
#include "backlight.h"
#include "atmosphere.h"
#include "modems.h"
#include "networks.h"
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
    Modems *modem = new Modems(&connection);
    Networks *network = new Networks(&connection);
    
    new BacklightAdaptor(backlight);
    new AtmosphereAdaptor(atmosphere);
    new ModemsAdaptor(modem);
    new NetworksAdaptor(network);

    connection.registerObject("/backlight", backlight);
    connection.registerObject("/atmosphere", atmosphere);
    connection.registerObject("/modem", modem);
    connection.registerObject("/connection", network);

    connection.registerService("org.cutie_shell.SettingsDaemon");

    return app.exec();
}
