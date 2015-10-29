QT       += core gui widgets sql
include(../../../g/g/g.pri)
LIBS += -lpcap
DESTDIR = $${PWD}/../bin
SOURCES += *.cpp
HEADERS += *.h
FORMS += *.ui
RESOURCES += cs.qrc
