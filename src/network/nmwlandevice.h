#ifndef NMWLANDEVICE_H
#define NMWLANDEVICE_H

#include <QDebug>
#include <iostream>
#include <QDBusInterface>
#include <QDBusReply>
#include "structures.h"
#include "networkdevice.h"
#include "nmaccesspoint.h"
#include "cutie_shell_adaptor.h"

class NMAccessPoint;

class NMWlanDevice : public NetworkDevice
{
    Q_OBJECT
public:
    NMWlanDevice(QDBusConnection *connection, QDBusObjectPath path);
private:
    QDBusObjectPath m_path;
    QDBusInterface m_device;
    QMap<QString, NMAccessPoint *> m_aps_by_path;
    QMap<QString, QDBusObjectPath> m_pathMap;
public Q_SLOTS:
    void onAPAdded(QDBusObjectPath path);
    void onPropertiesChanged(
	QString iface, 
	QMap<QString, QVariant> changed, 
	QStringList invalidated);
};

#endif // NMWLANDEVICE_H