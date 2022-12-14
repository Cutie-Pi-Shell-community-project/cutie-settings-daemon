#include "networkbackend.h"

int NetworkBackend::next_id = 1;

NetworkBackend::NetworkBackend(QDBusConnection *connection)
	: m_connection(connection) {}


QList<QDBusObjectPath> NetworkBackend::devicePaths() {
	return m_devicePaths;
}