#include "nmwlandevice.h"

NMWlanDevice::NMWlanDevice(QDBusConnection *connection, QDBusObjectPath path) 
    : NetworkDevice(connection), m_path(path), m_device(
	    "org.freedesktop.NetworkManager",
        path.path(), 
	    "org.freedesktop.NetworkManager.Device.Wireless",
        QDBusConnection::systemBus()
    ) {
    QDBusReply<QList<QDBusObjectPath>> apsReply = m_device.call(QLatin1String("GetAccessPoints"));
    QList<QDBusObjectPath> aps = apsReply.value();

    for (int i = 0; i < aps.size(); i++) {
        onAPAdded(aps.at(i));
    }

    QDBusReply<QDBusVariant> activeAPReply = QDBusInterface(
	    "org.freedesktop.NetworkManager",
        path.path(), 
	    "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus()
    ).call(
        QLatin1String("Get"), 
        QString("org.freedesktop.NetworkManager.Device.Wireless"),
        QString("ActiveAccessPoint")
    );

    QDBusObjectPath nmPath = activeAPReply.value().variant().value<QDBusObjectPath>();
    m_active_service = m_aps_by_path.value(nmPath.path());
    m_active_service_path = m_pathMap.value(nmPath.path());
    emit ActiveServiceChanged(m_active_service_path);

    connection->connect(
        "org.freedesktop.NetworkManager",
        m_path.path(), 
        "org.freedesktop.NetworkManager.Device.Wireless", 
        "AccessPointAdded", 
        this, SLOT(onAPAdded(QDBusObjectPath))
    );

    connection->connect(
        "org.freedesktop.NetworkManager",
        m_path.path(), 
        "org.freedesktop.NetworkManager.Device.Wireless", 
        "AccessPointRemoved", 
        this, SLOT(onAPRemoved(QDBusObjectPath))
    );

    connection->connect(
        "org.freedesktop.NetworkManager",
        m_path.path(), 
        "org.freedesktop.DBus.Properties", 
        "PropertiesChanged", 
        this, SLOT(onPropertiesChanged(QString,QMap<QString, QVariant>,QStringList))
    );
}

void NMWlanDevice::onAPAdded(QDBusObjectPath path) {
    NMAccessPoint *newAP = new NMAccessPoint(m_connection, path);
    m_aps_by_path.insert(path.path(), newAP);
    m_services.append(newAP);
    new NetworkServiceAdaptor(newAP);
    QString newPath = QString("/networking/service/").append(
        QString::number(NetworkDevice::next_id++));
    m_connection->registerObject(newPath, newAP);
    m_apPaths.append(QDBusObjectPath(newPath));
    m_pathMap.insert(path.path(), QDBusObjectPath(newPath));
    emit ServicesChanged(m_apPaths);
}

void NMWlanDevice::onAPRemoved(QDBusObjectPath path) {
    m_services.removeAll(m_aps_by_path.value(path.path()));
    m_apPaths.removeAll(m_pathMap.value(path.path()));
    m_connection->unregisterObject(m_pathMap.value(path.path()).path());
    m_pathMap.remove(path.path());
    NMAccessPoint *rem = m_aps_by_path.value(path.path());
    m_aps_by_path.remove(path.path());
    delete rem;
    emit ServicesChanged(m_apPaths);
}

void NMWlanDevice::onPropertiesChanged(
	QString iface, 
	QMap<QString, QVariant> changed, 
	QStringList invalidated) {
    Q_UNUSED(invalidated);
    if (iface == "org.freedesktop.NetworkManager.Device.Wireless") {
        if (changed.contains(QString("ActiveAccessPoint"))) {
            QDBusObjectPath nmPath = changed["ActiveAccessPoint"].value<QDBusObjectPath>();
            if (nmPath.path() == "/") {
                m_active_service = nullptr;
                m_active_service_path = QDBusObjectPath(QString("/"));
            } else {
                m_active_service = m_aps_by_path.value(nmPath.path());
                m_active_service_path = m_pathMap.value(nmPath.path());
            }
            emit ActiveServiceChanged(m_active_service_path);
        }
    }
}