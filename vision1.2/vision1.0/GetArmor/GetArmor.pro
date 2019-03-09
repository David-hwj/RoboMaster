TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    com.cpp \
    getarmor_rgb.cpp \
    getarmor_hsv.cpp \
    rgb_and_hsv.cpp

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so\
/usr/local/lib/libopencv_imgcodecs.so.3.4\
/usr/local/lib/libopencv_shape.so \
/usr/local/lib/libopencv_videoio.so

HEADERS += \
    com.h \
    getarmor_rgb.h \
    getarmor_hsv.h \
    rgb_and_hsv.h
