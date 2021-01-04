cmake_minimum_required(VERSION 3.6.0)

project(4fann LANGUAGES C CXX)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src/ui")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ExternalProject)
ExternalProject_Add(fann
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fann
    BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fann/build-release/${CMAKE_C_COMPILER_ID}-${CMAKE_C_COMPILER_VERSION}-${CMAKE_SIZEOF_VOID_P}
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -G ${CMAKE_GENERATOR} ../..
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --target all
    BUILD_ALWAYS TRUE
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    )

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
  OPTION(DISABLE_PARALLEL_FANN "Disable parallel fann functions" OFF)
ENDIF(OPENMP_FOUND)

find_package(Qt5 COMPONENTS Widgets Concurrent Printsupport)
#core gui widgets concurrent printsupport
set(SRC_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qcustomplot-source/qcustomplot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qcustomplot-source/qcustomplot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/netpanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/netpanel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/netproject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/netproject.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nettrainview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nettrainview.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/neuralnetwork.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/neuralnetwork.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/start_train_ann_dialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/start_train_ann_dialog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/thread_count_change_dialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/thread_count_change_dialog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/trainingdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/trainingdata.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/netpanel.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/thread_count_change_dialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/4fann.qrc
    )

if(WIN32)
    SET(GUI_TYPE WIN32)
endif(WIN32)

add_executable(${PROJECT_NAME} ${GUI_TYPE} ${SRC_LIST})
add_dependencies(${PROJECT_NAME} fann)
target_link_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fann/build-release/${CMAKE_C_COMPILER_ID}-${CMAKE_C_COMPILER_VERSION}-${CMAKE_SIZEOF_VOID_P}/src
    )
target_link_libraries (${PROJECT_NAME}
    Qt5::Core Qt5::Widgets Qt5::Concurrent Qt5::PrintSupport
    doublefann_static
    ${OpenMP_CXX_LIBRARIES}
    )
target_include_directories(${PROJECT_NAME}
        PRIVATE
            src
            src/ui
            3rdparty/qcustomplot-source
            3rdparty/fann/src/include
    )
#FANN_NO_DLL
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        FANN_NO_DLL
    )
target_compile_options(${PROJECT_NAME}
    PRIVATE
        $<$<BOOL:OPENMP_FOUND>:${OpenMP_CXX_FLAGS}>
    )
