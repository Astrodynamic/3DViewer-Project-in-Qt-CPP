cmake_minimum_required(VERSION 3.5)

project(Viewer3D VERSION 2.2 LANGUAGES CXX C)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  assimp
  URL https://github.com/assimp/assimp/archive/master.zip
)

FetchContent_GetProperties(assimp)

if(NOT assimp_POPULATED)
  FetchContent_Populate(assimp)
  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(ASSIMP_NO_EXPORT ON CACHE BOOL "")
  set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "")
  set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "")
  set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "")
  add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
  if (WIN32)
      set(ASSIMP_OPT_BUILD_PACKAGES  ON CACHE BOOL "" FORCE)
      set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
  endif()
endif()

include_directories(
  ${CMAKE_SOURCE_DIR}/application/mesh
  ${CMAKE_SOURCE_DIR}/application/camera
  ${CMAKE_SOURCE_DIR}/application/model
  ${CMAKE_SOURCE_DIR}/application/settings
  ${CMAKE_SOURCE_DIR}/application/controler
  ${CMAKE_SOURCE_DIR}/widgets/wgt_width
  ${CMAKE_SOURCE_DIR}/widgets/wgt_dialog_format
  ${CMAKE_SOURCE_DIR}/application/opengl
  ${CMAKE_SOURCE_DIR}/application/settings/model_settings
  ${CMAKE_SOURCE_DIR}/application/settings
  ${CMAKE_SOURCE_DIR}/application/savior
  ${CMAKE_SOURCE_DIR}/lib/gif_maker/3rdParty/giflib
  ${CMAKE_SOURCE_DIR}/lib/gif_maker/gifimage
  ${CMAKE_SOURCE_DIR}/application/scene
  ${CMAKE_SOURCE_DIR}/application/information/model_information
  ${CMAKE_SOURCE_DIR}/application/information/mesh_information
  ${CMAKE_SOURCE_DIR}/application/light
)

set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
  Widgets
  Gui
)

if(WIN32)
    find_package(OpenGL  REQUIRED)
    find_package( Qt5Core REQUIRED )
    find_package( Qt5Widgets REQUIRED )
    find_package( Qt5Gui REQUIRED )
    find_package( Qt5OpenGL REQUIRED )
else()
  find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS OpenGLWidgets)
  find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Widgets
  Gui
)

set(PROJECT_SOURCES
    application/main.cc
    application/opengl/v3d_gl.cc
    application/opengl/v3d_gl.h
    application/camera/camera.h
    application/camera/camera.cc
    application/mesh/mesh.h
    application/mesh/mesh.cc
    application/model/model.cc
    application/model/model.h
    application/controler/mainwindow.h
    application/controler/mainwindow.cc
    application/settings/model_settings/model_settings.h
    application/settings/global_settings.h
    application/savior/savior.h
    application/savior/savior.cc

    application/scene/scene.cc
    application/scene/scene.h

    application/light/light.cc
    application/light/light.h
    application/light/point_light.h
    application/light/spot_light.h
    application/light/illumination.h
    application/light/illumination.cc

    application/information/model_information/model_information.h
    application/information/mesh_information/mesh_information.h
    widgets/wgt_width/wgt_width.cc
    widgets/wgt_width/wgt_width.h
    widgets/wgt_dialog_format/dialog_format.cc
    widgets/wgt_dialog_format/dialog_format.h

    forms/wgt_width.ui
    forms/mainwindow.ui
    forms/dialog_format.ui

    lib/gif_maker/gifimage/qgifglobal.h
    lib/gif_maker/gifimage/qgifimage.h
    lib/gif_maker/gifimage/qgifimage_p.h

    lib/gif_maker/3rdParty/giflib/dgif_lib.c
    lib/gif_maker/3rdParty/giflib/egif_lib.c
    lib/gif_maker/3rdParty/giflib/gif_err.c
    lib/gif_maker/3rdParty/giflib/gif_hash.c
    lib/gif_maker/3rdParty/giflib/gifalloc.c
    lib/gif_maker/3rdParty/giflib/quantize.c

    lib/gif_maker/gifimage/qgifimage.cpp
)

if (APPLE)
  set(MACOSX_BUNDLE_ICON_FILE appicon.png)
  set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/data/icon/appicon.png")
  set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_macos}
)
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
    )
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
    )
    endif()
endif()

if (WIN32)
    qt5_add_resources(PROJECT_SOURCES
        shaders/shader.frag
        shaders/shader.vert
        shaders/scene.frag
        shaders/scene.vert
        shaders/vertex.vert
        shaders/vertex.frag
        shaders/edge.vert
        shaders/edge.frag
        shaders/edge.geom
        shaders/material.vert
        shaders/material.frag
        shaders/material_flat.vert
        shaders/material_flat.frag
        shaders/shader_flat.vert
        shaders/shader_flat.frag
        shaders/cubemap.vert
        shaders/cubemap.frag
        )

    qt5_add_resources(PROJECT_SOURCES
      data/skybox/back.jpg
      data/skybox/bottom.jpg
      data/skybox/front.jpg
      data/skybox/left.jpg
      data/skybox/right.jpg
      data/skybox/top.jpg
      )

else()
    qt_add_resources(${PROJECT_NAME} "shaders"
        PREFIX "/"
        BASE "shaders"
        FILES
          shaders/shader.frag
          shaders/shader.vert
          shaders/scene.frag
          shaders/scene.vert
          shaders/vertex.vert
          shaders/vertex.frag
          shaders/edge.vert
          shaders/edge.frag
          shaders/edge.geom
          shaders/material.vert
          shaders/material.frag
          shaders/material_flat.vert
          shaders/material_flat.frag
          shaders/shader_flat.vert
          shaders/shader_flat.frag
          shaders/cubemap.vert
          shaders/cubemap.frag
    )
    qt_add_resources(${PROJECT_NAME} "skybox"
    PREFIX "/"
    BASE "data/skybox"
    FILES
      data/skybox/back.jpg
      data/skybox/bottom.jpg
      data/skybox/front.jpg
      data/skybox/left.jpg
      data/skybox/right.jpg
      data/skybox/top.jpg
)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Gui
  assimp
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::OpenGL
        Qt5::Widgets
        Qt5::Core)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

set(PROJECT_TRDPARTY_DIR lib)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
    string(FIND ${SOURCE_FILE} ${PROJECT_TRDPARTY_DIR} PROJECT_TRDPARTY_DIR_FOUND)
    if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
        list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
    endif ()
endforeach ()

add_custom_target(
        cppcheck
        COMMAND cppcheck
        --enable=all
        --inconclusive
        --language=c++
        --std=c++17
        --verbose
        --suppress=missingInclude
        --suppress=unusedFunction
        --suppress=unusedStructMember
        --suppress=functionConst
        --suppress=functionStatic
        --suppress=uninitMemberVar
        --suppress=useStlAlgorithm
        --suppress=noExplicitConstructor
        --suppress=unmatchedSuppression
        --quiet
        ${ALL_SOURCE_FILES}
)

add_custom_target(
        clangformat
        COMMAND clang-format
        -style=Google
        -i
        --verbose
        ${ALL_SOURCE_FILES}
)

add_custom_target(
        style
        COMMAND clang-format
        -style=Google
        -n
        --verbose
        ${ALL_SOURCE_FILES}
)
