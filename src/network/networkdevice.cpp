#include "networkdevice.h"

int NetworkDevice::next_id = 1;

NetworkDevice::NetworkDevice(QDBusConnection *connection)
	: m_connection(connection) {}


NetworkConnection *NetworkDevice::getActiveService() {
	return m_active_service;
}

QList<QDBusObjectPath> NetworkDevice::apPaths() {
	return m_apPaths;
}

QDBusObjectPath NetworkDevice::activeServicePath() {
	return m_active_service_path;
}