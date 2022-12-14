QT += core dbus

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/backlight.cpp \
        src/atmosphere.cpp \
        src/modem/modems.cpp \
        src/modem/ofono_modem.cpp \
        src/network/networkmanager.cpp \
        src/network/nmwlandevice.cpp \
        src/network/nmaccesspoint.cpp \
        src/network/networkbackend.cpp \
        src/network/networkdevice.cpp \
        src/network/networkconnection.cpp \
        src/network/networking.cpp \
        src/main.cpp

HEADERS += \
        src/backlight.h \
        src/atmosphere.h \
        src/modem/modems.h \
        src/modem/ofono_modem.h \
        src/network/networkmanager.h \
        src/network/nmwlandevice.h \
        src/network/nmaccesspoint.h \
        src/network/networkbackend.h \
        src/network/networkdevice.h \
        src/network/networkconnection.h \
        src/network/networking.h \
        src/structures.h

INCLUDEPATH += src src/modem src/network

LIBS += -ludev

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

dbusdaemon.files = src/org.cutie_shell.xml
dbusdaemon.header_flags = -i backlight.h \
	-i atmosphere.h -i modems.h \
	-i networking.h

DBUS_ADAPTORS += \
        dbusdaemon 

DBUS_INTERFACES += \
        dbusdaemon \
        src/modem/ofono.xml \
        src/modem/ofono_modem.xml

dbuspolicy.files = data/org.cutie_shell.SettingsDaemon.conf
dbuspolicy.path = /usr/share/dbus-1/system.d/

systemdservice.files = data/cutie-settings-daemon.service
systemdservice.path = /usr/lib/systemd/system/

INSTALLS += dbuspolicy systemdservice