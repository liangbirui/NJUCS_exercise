QT       += core gui widgets sql webenginewidgets webview webkitwidgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = bin
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj

SOURCES += \
    insert.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    database.hpp \
    insert.h \
    mainwindow.h \
    md5.hpp \
    similarity.hpp

FORMS += \
    insert.ui \
    mainwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
