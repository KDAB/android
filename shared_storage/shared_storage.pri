equals(QT_MAJOR_VERSION, 5): QT += androidextras
equals(QT_MAJOR_VERSION, 6): QT += core-private

INCLUDEPATH += $$PWD

SOURCES += \
        $$PWD/contentresolver.cpp \
        $$PWD/cursor.cpp \
        $$PWD/documentfile.cpp \
        $$PWD/documentscontract.cpp \
        $$PWD/sharedstorage.cpp \
        $$PWD/utils.cpp

HEADERS += \
        $$PWD/contentresolver.h \
        $$PWD/cursor.h \
        $$PWD/documentfile.h \
        $$PWD/documentscontract.h \
        $$PWD/sharedstorage.h \
        $$PWD/utils.h
