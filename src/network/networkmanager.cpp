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
            new NetworkDeviceAdaptor(newDev);
            QString path = QString("/networking/device/").append(
                QString::number(NetworkBackend::next_id++));
            m_connection->registerObject(path, newDev);
            m_devicePaths.append(QDBusObjectPath(path));
            emit DevicesChanged(m_devicePaths);
        }
    }
}