#include <QCoreApplication>
#include "backlight.h"
#include "atmosphere.h"
#include "modems.h"
#include "networking.h"
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
    Networking *network = new Networking(&connection);
    
    new BacklightAdaptor(backlight);
    new AtmosphereAdaptor(atmosphere);
    new ModemsAdaptor(modem);
    new NetworkingAdaptor(network);

    connection.registerObject("/backlight", backlight);
    connection.registerObject("/atmosphere", atmosphere);
    connection.registerObject("/modem", modem);
    connection.registerObject("/networking", network);

    connection.registerService("org.cutie_shell.SettingsDaemon");

    return app.exec();
}
