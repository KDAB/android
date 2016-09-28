TEMPLATE = lib
TARGET = server
CONFIG += dll
QT += core
QT += remoteobjects

SOURCES += \
    server.cpp

REPC_SOURCE += pingpong.rep
