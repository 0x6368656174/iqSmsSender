#-------------------------------------------------
#
# Project created by QtCreator 2013-04-16T14:23:08
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = smssender
TEMPLATE = app


SOURCES += main.cpp\
        smssendermainwindow.cpp \
    iqdatabasesearcher.cpp \
    iqsettingmanager.cpp \
    iqhistoryloger.cpp \
    iqsmssender.cpp \
    iqsmpp/iqsmpp.cpp \
    iqsmpp/iqsmppbindoptions.cpp \
    iqsmpp/iqsmppabstractpdu.cpp \
    iqsmpp/iqsmppbindpdu.cpp \
    iqsmpp/iqsmppunbindpdu.cpp \
    iqsmpp/iqsmppsubmitsmpdu.cpp \
    iqsmpp/iqsmppsubmitsmoptions.cpp \
    iqsmpp/iqsmppesme.cpp \
    iqsmpp/iqsmppgenericnackpdu.cpp \
    iqsmpp/iqsmppenquirelinkpdu.cpp \
    iqsmpp/iqsmppenquirelinkresppdu.cpp \
    iqsmpp/iqsmppbindresppdu.cpp \
    iqsmpp/iqsmppbindrespoptions.cpp \
    iqsmpp/iqsmppunbindresppdu.cpp \
    iqsmpp/iqsmppabstractoptionalparameter.cpp \
    iqsmpp/iqsmppoptionalparameter.cpp \
    iqsmpp/iqsmppabstractoptions.cpp \
    iqwebaccesserver.cpp \
    iqdatabasetrailer.cpp \
    iqstaticfiletrailer.cpp

HEADERS  += smssendermainwindow.h \
    iqdatabasesearcher.h \
    iqsettingmanager.h \
    iqhistoryloger.h \
    iqsmssender.h \
    iqsmpp/iqsmpp.h \
    iqsmpp/iqsmppbindoptions.h \
    iqsmpp/iqsmppabstractpdu.h \
    iqsmpp/iqsmppbindpdu.h \
    iqsmpp/iqsmppunbindpdu.h \
    iqsmpp/iqsmppsubmitsmpdu.h \
    iqsmpp/iqsmppsubmitsmoptions.h \
    iqsmpp/iqsmppesme.h \
    iqsmpp/iqsmppgenericnackpdu.h \
    iqsmpp/iqsmppenquirelinkpdu.h \
    iqsmpp/iqsmppenquirelinkresppdu.h \
    iqsmpp/iqsmppbindresppdu.h \
    iqsmpp/iqsmppbindrespoptions.h \
    iqsmpp/iqsmppunbindresppdu.h \
    iqsmpp/iqsmppoptionalparameter.h \
    iqsmpp/iqsmppabstractoptionalparameter.h \
    iqsmpp/iqsmppabstractoptions.h \
    iqwebaccesserver.h \
    iqdatabasetrailer.h \
    iqstaticfiletrailer.h

FORMS    += smssendermainwindow.ui

RESOURCES += \
    icons.qrc

TRANSLATIONS += \
    smssender_ru.ts

RC_FILE = smssender.rc

