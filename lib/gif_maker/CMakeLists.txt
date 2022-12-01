cmake_minimum_required(VERSION 3.5)

project(qgifimage VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ${CMAKE_SOURCE_DIR}/3rdParty/giflib
  ${CMAKE_SOURCE_DIR}/gifimage
)

# set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
  Widgets
  OpenGLWidgets
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Widgets
  OpenGLWidgets
)

SET(GIFIMAGE_HEADERS
    gif_hash.h
    gif_lib.h
    gif_lib_private.h
    qgifglobal.h
    qgifimage.h
    qgifimage_p.h
)

SET(GIFIMAGE_SOURCE
    gif_font.c
    dgif_lib.c
    egif_lib.c
    gif_err.c
    gif_hash.c
    gifalloc.c
    quantize.c
    qgifimage.cpp
)


