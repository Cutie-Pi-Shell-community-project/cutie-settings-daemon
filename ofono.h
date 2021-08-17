#ifndef OFONO_H
#define OFONO_H

#include <QDebug>
#include <iostream>

#include "ofono_interface.h"
#include "ofono_modem_interface.h"

typedef QPair<QDBusObjectPath, QVariantMap> ofonoPair;
typedef QList<ofonoPair> OfonoServices;

Q_DECLARE_METATYPE(ofonoPair)
Q_DECLARE_METATYPE(OfonoServices)

class Ofono : public QObject
{
    Q_OBJECT
private:
    org::ofono::Manager *ofono;
    QList<org::ofono::Modem *> *ofonoModems;
    QList<org::ofono::NetworkRegistration *> *ofonoNetworks;
public:
    Ofono();
public Q_SLOTS:
};

#endif // OFONO_H