#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QDebug>
#include <QDBusObjectPath>

typedef QPair<QDBusObjectPath, QVariantMap> ServicePair;
typedef QList<ServicePair> ServiceList;

Q_DECLARE_METATYPE(ServicePair)
Q_DECLARE_METATYPE(ServiceList)

#endif