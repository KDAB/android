QT += quick

include($$PWD/../../shared_storage/shared_storage.pri)

SOURCES += \
        documentscontroller.cpp \
        main.cpp

HEADERS += \
        documentscontroller.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

