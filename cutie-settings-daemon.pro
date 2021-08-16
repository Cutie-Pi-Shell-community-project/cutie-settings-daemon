QT += core dbus

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        backlight.cpp \
        atmosphere.cpp \
        main.cpp

HEADERS += \
        backlight.h \
        atmosphere.h 

LIBS += -ludev

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

dbusdaemon.files = com.github.CutiePiShellCommunityProject.xml
dbusdaemon.header_flags = -i backlight.h -i atmosphere.h 

DBUS_ADAPTORS += \
        dbusdaemon 

DBUS_INTERFACES += \
        dbusdaemon 

dbuspolicy.files = com.github.CutiePiShellCommunityProject.SettingsDaemon.conf
dbuspolicy.path = /usr/share/dbus-1/system.d/

systemdservice.files = cutie-settings-daemon.service
systemdservice.path = /usr/lib/systemd/system/

INSTALLS += dbuspolicy systemdservice