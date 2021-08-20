#include <QCoreApplication>
#include "backlight.h"
#include "atmosphere.h"
#include "ofono.h"
#include "cutie_shell_adaptor.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QDBusConnection connection = QDBusConnection::systemBus();

    Backlight *backlight = new Backlight();
    Atmosphere *atmosphere = new Atmosphere();
    Ofono *ofono = new Ofono(&connection);
    
    new BacklightAdaptor(backlight);
    new AtmosphereAdaptor(atmosphere);
    new OfonoAdaptor(ofono);

    connection.registerObject("/backlight", backlight);
    connection.registerObject("/atmosphere", atmosphere);
    connection.registerObject("/modem", ofono);

    connection.registerService("org.cutie_shell.SettingsDaemon");

    return app.exec();
}
