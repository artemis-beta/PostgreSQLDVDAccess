 QT       += core gui

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

ICON = icon.icns

SOURCES += \
    about.cxx \
    main.cpp \
    mainwindow.cxx \
    postgresql_connector.cxx \
    preferences.cxx \
    sqllogging.cxx

HEADERS += \
    about.hxx \
    mainwindow.hxx \
    media_db_structs.hxx \
    postgresql_connector.hxx \
    preferences.hxx \
    sqllogging.hxx

FORMS += \
    about.ui \
    deletebox.ui \
    mainwindow.ui \
    preferences.ui \
    sqllogging.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L/usr/local/lib/release/ -lsoci_postgresql
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/local/lib/debug/ -lsoci_postgresql
else:unix: LIBS += -L/usr/local/lib/ -lsoci_core -lsoci_postgresql

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/soci/postgresql
DEPENDPATH += /usr/local/include
DEPENDPATH += /usr/local/include/soci/postgresql

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/local/lib/release/libsoci_postgresql.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/local/lib/debug/libsoci_postgresql.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/local/lib/release/soci_postgresql.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/local/lib/debug/soci_postgresql.lib
else:unix: PRE_TARGETDEPS += /usr/local/lib/libsoci_postgresql.a

RESOURCES += \
    PostgreSQLDVDAccess.qrc
