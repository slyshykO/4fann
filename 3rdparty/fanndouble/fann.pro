
QT -= core
TEMPLATE = lib
CONFIG += static

DESTDIR = ./fann/bin

CONFIG(debug, debug|release) {
    CONFIG += console
    TARGET = fannd
    OBJECTS_DIR = build/debug
    MOC_DIR = build/debug
}
else {
    TARGET = fann
    OBJECTS_DIR = build/release
    MOC_DIR = build/release
}


INCLUDEPATH += ./fann/src/include

HEADERS += \
    fann/src/include/config.h \
    fann/src/include/doublefann.h \
    fann/src/include/fann.h \
    fann/src/include/fann_activation.h \
    fann/src/include/fann_cascade.h \
    fann/src/include/fann_cpp.h \
    fann/src/include/fann_data.h \
    fann/src/include/fann_error.h \
    fann/src/include/fann_internal.h \
    fann/src/include/fann_io.h \
    fann/src/include/fann_train.h \
    fann/src/include/fixedfann.h \
    fann/src/include/floatfann.h

SOURCES += \
    fann/src/doublefann.c


DEFINES += DOUBLEFANN

