#ifndef MODEMS_H
#define MODEMS_H

#include <QDebug>
#include <iostream>

#include "structures.h"

#include "ofono_interface.h"
#include "ofono_modem_interface.h"
#include "ofono_modem.h"


class Modems : public QObject
{
    Q_OBJECT
private:
    org::ofono::Manager *ofono;
    QList<OfonoModem *> *ofonoModemList;
    QDBusConnection *connection;
public:
    Modems(QDBusConnection *connection);
public Q_SLOTS:
    unsigned int ModemCount();
    void onOfonoModemAdded(QDBusObjectPath path, QVariantMap properties);
Q_SIGNALS:
    void ModemAdded(QDBusObjectPath path);
};

#endif // MODEM_H