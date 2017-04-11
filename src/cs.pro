QT       += core gui widgets sql multimedia
include(../../g/g.pri)
LIBS += -lpcap
DESTDIR = $${PWD}/../bin
SOURCES += *.cpp
HEADERS += *.h
FORMS += *.ui
RESOURCES += cs.qrc
