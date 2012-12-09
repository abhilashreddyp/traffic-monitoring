QT += core

TARGET = traffic_lib
TEMPLATE = lib

HEADERS += \
    frame_processor.hpp \
    video_processor.hpp \
    utility.hpp \
    data_istream.hpp \
    exception.hpp \
    image_loader.hpp \
    video.hpp

SOURCES += \
    video_processor.cpp \
    video.cpp

LIBS += -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc
