QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DESTDIR = $$PWD/bin
SOURCES += \
    itemwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    settingwidget.cpp \
    settingwidget2.cpp \
    toast.cpp \
    toastmanager.cpp

HEADERS += \
    common_utils.h \
    itemwidget.h \
    mainwindow.h \
    settingwidget.h \
    settingwidget2.h \
    toast.h \
    toastmanager.h

FORMS += \
    itemwidget.ui \
    mainwindow.ui \
    settingwidget.ui \
    settingwidget2.ui \
    toast.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    re.qrc

DISTFILES +=
