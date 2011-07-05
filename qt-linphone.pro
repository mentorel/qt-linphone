#-------------------------------------------------
#
# Project created by QtCreator 2011-03-18T00:46:19
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = qt-linphone
TEMPLATE = app


SOURCES += main.cpp\
        qt-linphone.cpp \
    gui_config.cpp \
    addressbook.cpp \
    contact.cpp \
    listviewdelegate.cpp \
    linphone.cpp \
    statemachine.cpp

HEADERS  += qt-linphone.h \
    gui_config.h \
    addressbook.h \
    contact.h \
    listviewdelegate.h \
    linphone.h \
    statemachine.h

FORMS    += form.ui
