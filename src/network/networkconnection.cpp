#include "networkconnection.h"

NetworkConnection::NetworkConnection(QDBusConnection *connection)
	: m_connection(connection) {}
