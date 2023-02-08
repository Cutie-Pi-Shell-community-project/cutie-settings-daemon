cmake_minimum_required(VERSION 3.16)
project(cutie-settings-daemon VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus)
find_package(Libudev)

set(cutie-settings-daemon_SOURCES
    src/atmosphere.cpp src/atmosphere.h
    src/backlight.cpp src/backlight.h
    src/main.cpp
    src/modem/modems.cpp src/modem/modems.h
    src/modem/ofono_modem.cpp src/modem/ofono_modem.h
    src/network/networkbackend.cpp src/network/networkbackend.h
    src/network/networkconnection.cpp src/network/networkconnection.h
    src/network/networkdevice.cpp src/network/networkdevice.h
    src/network/networking.cpp src/network/networking.h
    src/network/networkmanager.cpp src/network/networkmanager.h
    src/network/nmaccesspoint.cpp src/network/nmaccesspoint.h
    src/network/nmwlandevice.cpp src/network/nmwlandevice.h
    src/structures.h
)

qt_add_dbus_interface(cutie-settings-daemon_SOURCES src/modem/ofono.xml ofono_interface)
qt_add_dbus_interface(cutie-settings-daemon_SOURCES src/modem/ofono_modem.xml ofono_modem_interface)
qt_add_dbus_adaptor(cutie-settings-daemon_SOURCES src/atmosphere.xml atmosphere.h Atmosphere AtmosphereAdaptor)
qt_add_dbus_adaptor(cutie-settings-daemon_SOURCES src/backlight.xml backlight.h Backlight BacklightAdaptor)
qt_add_dbus_adaptor(cutie-settings-daemon_SOURCES src/modem.xml ofono_modem.h OfonoModem ModemAdaptor)
qt_add_dbus_adaptor(cutie-settings-daemon_SOURCES src/modems.xml modems.h Modems ModemsAdaptor)
qt_add_dbus_adaptor(cutie-settings-daemon_SOURCES src/networkbackend.xml networkbackend.h NetworkBackend NetworkBackendAdaptor)
qt_add_dbus_adaptor(cutie-settings-daemon_SOURCES src/networkdevice.xml networkdevice.h NetworkDevice NetworkDeviceAdaptor)
qt_add_dbus_adaptor(cutie-settings-daemon_SOURCES src/networking.xml networking.h Networking NetworkingAdaptor)
qt_add_dbus_adaptor(cutie-settings-daemon_SOURCES src/networkservice.xml networkconnection.h NetworkConnection NetworkServiceAdaptor)

qt_add_executable(cutie-settings-daemon
    ${cutie-settings-daemon_SOURCES}
)

target_include_directories(cutie-settings-daemon PUBLIC
    src
    src/modem
    src/network
)

target_link_libraries(cutie-settings-daemon PUBLIC
    PkgConfig::Libudev
    Qt::Core
    Qt::DBus
)

install(TARGETS cutie-settings-daemon RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES data/org.cutie_shell.SettingsDaemon.conf DESTINATION "${CMAKE_INSTALL_DATADIR}/dbus-1/system.d/")
