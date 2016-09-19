TEMPLATE = app

QT += qml quick

QT += androidextras

SOURCES += main.cpp \
    qandroidmediaplayer.cpp \
    qsurfacetexture.cpp

HEADERS += \
    qandroidmediaplayer.h \
    qsurfacetexture.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    android/src/com/kdab/android/SurfaceTextureListener.java \
    android/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
