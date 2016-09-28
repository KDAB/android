QT += qml quick
QT += androidextras
QT += remoteobjects
CONFIG += c++11

SOURCES += client.cpp

REPC_REPLICA += pingpong.rep

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/com/kdab/training/MyService.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
