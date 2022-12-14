#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <QDebug>
#include <iostream>
#include <QDBusConnection>
#include "structures.h"

class NetworkConnection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString DisplayName READ getDisplayName NOTIFY DisplayNameChanged)
    Q_PROPERTY(uchar Strength READ getStrength NOTIFY StrengthChanged)
    Q_PROPERTY(bool Wireless READ isWireless NOTIFY IsWirelessChanged)
protected:
    QDBusConnection *m_connection;
public:
    NetworkConnection(QDBusConnection *connection);
    virtual QString getDisplayName() = 0;
    virtual uchar getStrength() = 0;
    virtual bool isWireless() = 0;
signals:
    void DisplayNameChanged(QString name);
    void StrengthChanged(uchar strength);
    void IsWirelessChanged(bool wifi);
};

#endif // NETWORK_CONNECTION_H