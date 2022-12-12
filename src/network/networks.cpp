#include "networks.h"
#include "cutie_shell_adaptor.h"

Networks::Networks(QDBusConnection *connection)
{
    this->connection = connection;

    this->connman = new Connman(connection);
}

uchar Networks::GetWifiStrength() {
    return connman->getWifiStrength();
}

QString Networks::GetWifiName() {
    return connman->getWifiName();
}