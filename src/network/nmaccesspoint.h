#ifndef NMACCESSPOINT_H
#define NMACCESSPOINT_H

#include <QDebug>
#include <iostream>
#include <QDBusInterface>
#include <QDBusReply>
#include "structures.h"
#include "networkconnection.h"

class NMAccessPoint : public NetworkConnection
{
    Q_OBJECT
public:
    NMAccessPoint(QDBusConnection *connection, QDBusObjectPath path);
    QString getDisplayName() override;
    uchar getStrength() override;
    bool isWireless() override;
private:
    QDBusObjectPath m_path;
    QDBusInterface m_ap;
    QString m_ssid;
    uchar m_strength;
public Q_SLOTS:
    void onPropertiesChanged(
	QString iface, 
	QMap<QString, QVariant> changed, 
	QStringList invalidated);
};

#endif // NMACCESSPOINT_H