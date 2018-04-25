TEMPLATE = lib
TARGET = server
CONFIG += dll
QT += core
QT += remoteobjects
QT += androidextras

SOURCES += \
    server.cpp

REPC_SOURCE += pingpong.rep
