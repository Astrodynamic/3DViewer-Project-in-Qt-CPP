cmake_minimum_required(VERSION 3.15)

project(Viewer3D VERSION 1.0.0 LANGUAGES CXX C)

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

  if(WIN32)
    set(ASSIMP_OPT_BUILD_PACKAGES ON CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
  endif()
endif()

set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
  Widgets
  Gui
)

if(WIN32)
  find_package(OpenGL REQUIRED)
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5OpenGL REQUIRED)
else()
  find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    OpenGLWidgets
  )

  find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    OpenGLWidgets
  )
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Widgets
  Gui
)

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
  ${CMAKE_SOURCE_DIR}/lib/giflib
  ${CMAKE_SOURCE_DIR}/lib/gifimage
  ${CMAKE_SOURCE_DIR}/application/scene
  ${CMAKE_SOURCE_DIR}/application/information/model_information
  ${CMAKE_SOURCE_DIR}/application/information/mesh_information
  ${CMAKE_SOURCE_DIR}/application/light
)

set(HEADERS
  ${CMAKE_SOURCE_DIR}/application/opengl/v3d_gl.h
  ${CMAKE_SOURCE_DIR}/application/camera/camera.h
  ${CMAKE_SOURCE_DIR}/application/mesh/mesh.h
  ${CMAKE_SOURCE_DIR}/application/model/model.h
  ${CMAKE_SOURCE_DIR}/application/controler/mainwindow.h
  ${CMAKE_SOURCE_DIR}/application/settings/model_settings/model_settings.h
  ${CMAKE_SOURCE_DIR}/application/settings/global_settings.h
  ${CMAKE_SOURCE_DIR}/application/savior/savior.h
  ${CMAKE_SOURCE_DIR}/application/scene/scene.h
  ${CMAKE_SOURCE_DIR}/application/light/light.h
  ${CMAKE_SOURCE_DIR}/application/light/point_light.h
  ${CMAKE_SOURCE_DIR}/application/light/spot_light.h
  ${CMAKE_SOURCE_DIR}/application/light/illumination.h
  ${CMAKE_SOURCE_DIR}/application/information/model_information/model_information.h
  ${CMAKE_SOURCE_DIR}/application/information/mesh_information/mesh_information.h
  ${CMAKE_SOURCE_DIR}/widgets/wgt_width/wgt_width.h
  ${CMAKE_SOURCE_DIR}/widgets/wgt_dialog_format/dialog_format.h
  ${CMAKE_SOURCE_DIR}/lib/gifimage/qgifglobal.h
  ${CMAKE_SOURCE_DIR}/lib/gifimage/qgifimage.h
  ${CMAKE_SOURCE_DIR}/lib/gifimage/qgifimage_p.h
)

set(SOURCES
  ${CMAKE_SOURCE_DIR}/application/opengl/v3d_gl.cc
  ${CMAKE_SOURCE_DIR}/application/camera/camera.cc
  ${CMAKE_SOURCE_DIR}/application/mesh/mesh.cc
  ${CMAKE_SOURCE_DIR}/application/model/model.cc
  ${CMAKE_SOURCE_DIR}/application/controler/mainwindow.cc
  ${CMAKE_SOURCE_DIR}/application/savior/savior.cc
  ${CMAKE_SOURCE_DIR}/application/scene/scene.cc
  ${CMAKE_SOURCE_DIR}/application/light/light.cc
  ${CMAKE_SOURCE_DIR}/application/light/illumination.cc
  ${CMAKE_SOURCE_DIR}/widgets/wgt_width/wgt_width.cc
  ${CMAKE_SOURCE_DIR}/widgets/wgt_dialog_format/dialog_format.cc
  ${CMAKE_SOURCE_DIR}/lib/giflib/dgif_lib.c
  ${CMAKE_SOURCE_DIR}/lib/giflib/egif_lib.c
  ${CMAKE_SOURCE_DIR}/lib/giflib/gif_err.c
  ${CMAKE_SOURCE_DIR}/lib/giflib/gif_hash.c
  ${CMAKE_SOURCE_DIR}/lib/giflib/gifalloc.c
  ${CMAKE_SOURCE_DIR}/lib/giflib/quantize.c
  ${CMAKE_SOURCE_DIR}/lib/gifimage/qgifimage.cpp
)

set(FORMS
  ${CMAKE_SOURCE_DIR}/forms/wgt_width.ui
  ${CMAKE_SOURCE_DIR}/forms/mainwindow.ui
  ${CMAKE_SOURCE_DIR}/forms/dialog_format.ui
)

set(PROJECT_SOURCES
  ${HEADERS}
  ${SOURCES}
  ${FORMS}
  application/main.cc
)

if(APPLE)
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

set(SHADERS_VERT
  shaders/shader.vert
  shaders/scene.vert
  shaders/vertex.vert
  shaders/edge.vert
  shaders/material.vert
  shaders/material_flat.vert
  shaders/shader_flat.vert
  shaders/cubemap.vert
)

set(SHADERS_FRAG
  shaders/shader.frag
  shaders/scene.frag
  shaders/vertex.frag
  shaders/edge.frag
  shaders/material.frag
  shaders/material_flat.frag
  shaders/shader_flat.frag
  shaders/cubemap.frag
)

set(SHADERS_GEOM
  shaders/edge.geom
)

set(SHADERS
  ${SHADERS_VERT}
  ${SHADERS_FRAG}
  ${SHADERS_GEOM}
)

set(SKYBOX
  data/skybox/back.jpg
  data/skybox/bottom.jpg
  data/skybox/front.jpg
  data/skybox/left.jpg
  data/skybox/right.jpg
  data/skybox/top.jpg
)

if(WIN32)
  qt5_add_resources(PROJECT_SOURCES
    ${SHADERS}
  )

  qt5_add_resources(PROJECT_SOURCES
    ${SKYBOX}
  )

else()
  qt_add_resources(${PROJECT_NAME} "shaders"
    PREFIX "/"
    BASE "shaders"
    FILES
    ${SHADERS}
  )
  qt_add_resources(${PROJECT_NAME} "skybox"
    PREFIX "/"
    BASE "data/skybox"
    FILES
    ${SKYBOX}
  )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Gui
  assimp
  -fsanitize=address
)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::OpenGL
    Qt5::Widgets
    Qt5::Core
  )
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
  )
endif()

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
  -Wall
  -Werror
  -Wextra
  -Wpedantic
  -fsanitize=address
  -Wno-error=deprecated-declarations
  -Wno-sign-compare
  -Wno-format-security
)

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

# Add cppcheck
find_program(CPPCHECK cppcheck)

if(CPPCHECK)
  message(STATUS "cppcheck found: ${CPPCHECK}")
  list(
    APPEND CPPCHECK_ARGS
    "--enable=all"
    "--inconclusive"
    "--quiet"
    "--language=c++"
    "--std=c++17"
    "--suppress=missingIncludeSystem"
    "--suppress=unusedFunction"
    "--suppress=missingInclude"
    "--suppress=unusedStructMember"
    "--suppress=variableScope"
    "--suppress=uninitMemberVar"
    "--suppress=*:${PROJECT_SOURCE_DIR}/lib/*"
  )
  add_custom_target(
    cppcheck
    COMMAND ${CPPCHECK} ${CPPCHECK_ARGS} ${HEADERS} ${SOURCES}
  )
else()
  message(STATUS "cppcheck not found")
endif()

# Add clang-format
find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
  message(STATUS "clang-format found: ${CLANG_FORMAT}")
  list(
    APPEND CLANG_ARGS
    "-i"
    "-style=Google"
    "--verbose"
  )
  add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT} ${CLANG_ARGS} ${HEADERS} ${SOURCES}
  )
else()
  message(STATUS "clang-format not found")
endif()
