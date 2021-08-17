#include <QCoreApplication>
#include "backlight.h"
#include "atmosphere.h"
#include "ofono.h"
#include "cutiepishellcommunityproject_adaptor.h"

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

    connection.registerObject("/com/github/CutiePiShellCommunityProject/backlight", backlight);
    connection.registerObject("/com/github/CutiePiShellCommunityProject/atmosphere", atmosphere);
    connection.registerObject("/com/github/CutiePiShellCommunityProject/modem", ofono);

    connection.registerService("com.github.CutiePiShellCommunityProject.SettingsDaemon");

    return app.exec();
}
