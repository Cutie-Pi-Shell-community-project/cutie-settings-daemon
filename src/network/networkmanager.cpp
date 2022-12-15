#include "networking.h"
#include "cutie_shell_adaptor.h"

NetworkManager::NetworkManager(QDBusConnection *connection)
	: NetworkBackend(connection), nm(
	    "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager", 
	    "org.freedesktop.NetworkManager",
        QDBusConnection::systemBus()
    ) {
    QDBusReply<QList<QDBusObjectPath>> devicesReply = nm.call(QLatin1String("GetDevices"));
    QList<QDBusObjectPath> devices = devicesReply.value();

    for (int i = 0; i < devices.size(); i++) {
        onDeviceAdded(devices.at(i));
    }

    connection->connect(
	    "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager", 
	    "org.freedesktop.NetworkManager", 
        "DeviceAdded", 
        this, SLOT(onDeviceAdded(QDBusObjectPath))
    );
}

void NetworkManager::onDeviceAdded(QDBusObjectPath path) {
    QDBusInterface devInterface(
        "org.freedesktop.NetworkManager",
        path.path(), 
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus()
    );
    QDBusReply<QDBusVariant> typeReply = devInterface.call(
        QLatin1String("Get"), 
        QString("org.freedesktop.NetworkManager.Device"),
        QString("DeviceType")
    );
    if (typeReply.isValid()) {
        uint type = typeReply.value().variant().toUInt();
        if (type == 2) { // Wireless LAN
            NetworkDevice *newDev = new NMWlanDevice(m_connection, path);
            m_devices.append(newDev);
            m_devicesByPath.insert(path.path(), newDev);
            new NetworkDeviceAdaptor(newDev);
            QString newpath = QString("/networking/device/").append(
                QString::number(NetworkBackend::next_id++));
            m_connection->registerObject(newpath, newDev);
            m_exportMap.insert(path.path(), QDBusObjectPath(newpath));
            m_devicePaths.append(QDBusObjectPath(newpath));
            emit DevicesChanged(m_devicePaths);
        }
    }
}

void NetworkManager::onDeviceRemoved(QDBusObjectPath path) {
    NetworkDevice *oldDev = m_devicesByPath.value(path.path());
    if (!oldDev) return;
    m_devices.removeAll(oldDev);
    QDBusObjectPath oldExport = m_exportMap.value(path.path());
    m_connection->unregisterObject(oldExport.path());
    m_devicePaths.removeAll(oldExport);
    m_exportMap.remove(path.path());
    emit DevicesChanged(m_devicePaths);
}