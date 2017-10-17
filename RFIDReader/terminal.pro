QT += widgets serialport

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    console.cpp \
    settings.cpp

HEADERS += \
    mainwindow.h \
    console.h \
    settings.h

FORMS += \
    mainwindow.ui \
    settings.ui

RESOURCES += \
    terminal.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialport/terminal
INSTALLS += target
