#-------------------------------------------------
#
# Project created by QtCreator 2019-01-06T21:13:30
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bazy
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    frontend/main.cpp \
    frontend/mainwindow.cpp \
    frontend/Equipment.cpp \
    backend/Database.cpp \
    frontend/AddEquipment.cpp \
    common.cpp \
    frontend/ClientsTab.cpp \
    frontend/AddClient.cpp \
    frontend/RentsTab.cpp \
    frontend/AddHire.cpp \
    frontend/ChooseEquipment.cpp \
    frontend/ChooseClient.cpp \
    frontend/EditClientDetails.cpp \
    frontend/EditEquipment.cpp

HEADERS += \
    frontend/mainwindow.h \
    frontend/Equipment.h \
    backend/Database.h \
    common.h \
    frontend/AddEquipment.h \
    frontend/ClientsTab.h \
    frontend/AddClient.h \
    frontend/RentsTab.h \
    frontend/AddHire.h \
    frontend/ChooseEquipment.h \
    frontend/ChooseClient.h \
    frontend/EditClientDetails.h \
    frontend/EditEquipment.h

FORMS += \
        mainwindow.ui \
    addequipment.ui \
    addclient.ui \
    addhire.ui \
    chooseequipment.ui \
    chooseclientwindow.ui \
    editclient.ui \
    editequipment.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
