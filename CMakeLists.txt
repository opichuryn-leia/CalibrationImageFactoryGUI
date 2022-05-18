cmake_minimum_required(VERSION 3.12)

set(BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LFS_TARGET CalibrationFactoryGUI)
project(${LFS_TARGET}Project LANGUAGES CXX)

list(APPEND QT_COMPONENTS Core Quick Gui)

find_package(QT NAMES Qt5 COMPONENTS ${QT_COMPONENTS} REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS} REQUIRED)

file(
    GLOB
    THIS_PROJECT_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

IF (WIN32)
    set(APP_EXEC_FLAG "WIN32")
    set(APP_RC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ui/images/appicon.rc)
endif()

add_executable(${LFS_TARGET}
    ${APP_EXEC_FLAG}
    ${APP_RC_FILE}
    ${THIS_PROJECT_SOURCE_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/fonts/fonts.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/images/images.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/qml/qml.qrc
)

set(QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ui/qml
    CACHE STRING "Qt Creator extra qml import paths"
    FORCE
)

# target_sources(${LFS_TARGET} PRIVATE ${THIS_PROJECT_SOURCE_FILES})

target_compile_definitions(${LFS_TARGET}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_directories(${LFS_TARGET} PRIVATE ${EXTRA_LIBS})
target_link_libraries(${LFS_TARGET}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
)

add_subdirectory(deploy)
