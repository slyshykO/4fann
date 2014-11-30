#-------------------------------------------------
#
#
#
#-------------------------------------------------

QT       += core gui widgets concurrent
CONFIG += c++11

TEMPLATE = app
DESTDIR = ./bin
UI_HEADERS_DIR = build
UI_DIR         = build
RCC_DIR        = build

CONFIG(debug, debug|release) {
    CONFIG += console
    TARGET = 4fann_d
    OBJECTS_DIR = build/debug
    MOC_DIR = build/debug
}
else {
    TARGET = 4fann
    OBJECTS_DIR = build/release
    MOC_DIR = build/release
}

include(src/plot/plot.pri)

# ==================== Подключаем Qwt ==============================
#QWT_ROOT - Переменная окружения, должна указывать директорию проекта Qwt
# =============================== Qwt ==============================
CONFIG(debug, debug|release) {
LIBS += -lqwtd
}
else{
LIBS += -lqwt
}
INCLUDEPATH += $$(QWT_ROOT)/src
LIBS += -L$$(QWT_ROOT)/lib

# ==================== Подключаем FANN ==============================
#FANN_DIR - Переменная окружения, должна указывать директорию проекта FANN
INCLUDEPATH += $$(FANN_DIR)/src/include
LIBS +=      -L$$(FANN_DIR)/bin
win32:{
CONFIG(debug, debug|release) {
    LIBS += -lfanndoubled
} else {
    LIBS += -lfanndouble
}
}
# ==============================  FANN ==============================

unix:{
LIBS += -lfann
}

#win32:RC_FILE = mystery_ai.rc

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/neuralnetwork.cpp \
    src/netproject.cpp \
    src/netpanel.cpp \
    src/trainingdata.cpp \
    src/start_train_ann_dialog.cpp \
    src/nettrainview.cpp \
    src/thread_count_change_dialog.cpp

HEADERS += \
    src/mainwindow.hpp \
    src/neuralnetwork.hpp \
    src/netproject.hpp \
    src/netpanel.hpp \
    src/trainingdata.hpp \
    src/start_train_ann_dialog.hpp \
    src/nettrainview.hpp \
    src/thread_count_change_dialog.hpp

FORMS += \
    src/ui/netpanel.ui \
    src/ui/thread_count_change_dialog.ui

RESOURCES += \
    4fann.qrc

unix:{
QMAKE_RPATHDIR += $$(FANN_DIR)/bin $$(QWT_ROOT)/lib
}
