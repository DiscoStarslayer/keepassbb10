APP_NAME = KeepassBerry

CONFIG += qt warn_on cascades10
INCLUDEPATH +=${QNX_TARGET}/usr/include/openssl ${QNX_TARGET}/usr/include/bb/data ${QNX_TARGET}/usr/include/qt4/QtCore ${QNX_TARGET}/usr/include/sbengine
DEPENDPATH +=${QNX_TARGET}/usr/include/openssl ${QNX_TARGET}/usr/include/bb/data ${QNX_TARGET}/usr/include/qt4/QtCore ${QNX_TARGET}/usr/include/sbengine 

LIBS += -lssl -lcrypto -lbbdata
QT += core

include(config.pri)
