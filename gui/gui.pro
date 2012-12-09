QT += core gui

TARGET = traffic-monitoring
TEMPLATE = app

SOURCES += \
    main.cpp \
    main_window.cpp

LIBS += -L../lib/. \
        -ltraffic_lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc

INCLUDEPATH += ../lib/.

HEADERS += \
    main_window.hpp

FORMS += \
    main_window.ui
