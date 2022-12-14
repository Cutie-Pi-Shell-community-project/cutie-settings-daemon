#include "nmaccesspoint.h"

NMAccessPoint::NMAccessPoint(QDBusConnection *connection, QDBusObjectPath path) 
    : NetworkConnection(connection), m_path(path), m_ap(
	    "org.freedesktop.NetworkManager",
        path.path(), 
	    "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus()
    ) {

    QDBusReply<QDBusVariant> ssidReply = m_ap.call(
        QLatin1String("Get"), 
        QString("org.freedesktop.NetworkManager.AccessPoint"),
        QString("Ssid")
    );

    QDBusReply<QDBusVariant> strengthReply = m_ap.call(
        QLatin1String("Get"), 
        QString("org.freedesktop.NetworkManager.AccessPoint"),
        QString("Strength")
    );

    m_ssid = ssidReply.value().variant().value<QString>();
    m_strength = strengthReply.value().variant().value<uchar>();

    emit DisplayNameChanged(m_ssid);
    emit StrengthChanged(m_strength);

    connection->connect(
        "org.freedesktop.NetworkManager",
        m_path.path(), 
        "org.freedesktop.DBus.Properties", 
        "PropertiesChanged", 
        this, SLOT(onPropertiesChanged(QString,QMap<QString, QVariant>,QStringList))
    );
}

void NMAccessPoint::onPropertiesChanged(
	QString iface, 
	QMap<QString, QVariant> changed, 
	QStringList invalidated) {
    Q_UNUSED(invalidated);
    if (iface == "org.freedesktop.NetworkManager.AccessPoint") {
        if (changed.contains(QString("Ssid"))) {
            m_ssid = changed["Ssid"].value<QString>();
    	    emit DisplayNameChanged(m_ssid);
        }
        if (changed.contains(QString("Strength"))) {
            m_strength = changed["Strength"].value<uchar>();
            emit StrengthChanged(m_strength);
        }
    }
}

QString NMAccessPoint::getDisplayName() {
    return m_ssid;
}

uchar NMAccessPoint::getStrength() {
    return m_strength;
}

bool NMAccessPoint::isWireless() {
    return true;
}