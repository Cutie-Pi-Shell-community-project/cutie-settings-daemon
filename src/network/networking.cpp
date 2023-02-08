#include "networking.h"
#include "NetworkBackendAdaptor.h"

Networking::Networking(QDBusConnection *connection)
    : connection(connection) {

    if (connection->interface()->serviceOwner("org.freedesktop.NetworkManager").isValid()) {
	NetworkBackend *newBackend = new NetworkManager(connection);
    	m_backends.append(newBackend);
	new NetworkBackendAdaptor(newBackend);
	QString path = QString("/networking/backend/").append(QString::number(1));
	connection->registerObject(path, newBackend);
	m_backendPaths.append(QDBusObjectPath(path));
	emit BackendsChanged(m_backendPaths);
    }
}

QList<QDBusObjectPath> Networking::backendPaths() {
	return m_backendPaths;
}