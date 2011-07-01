project(polyobject)

cmake_minimum_required(VERSION 2.6.0)

set(CMAKE_BUILD_TYPE Debug)

find_package(Qt4 COMPONENTS QtCore QtXml REQUIRED)
include(${QT_USE_FILE})

find_package(VTK REQUIRED)
if (VTK_FOUND)
    message(STATUS "Found VTK: " ${VTK_MAJOR_VERSION} "." ${VTK_MINOR_VERSION} "." ${VTK_BUILD_VERSION})
    include(${VTK_USE_FILE})
endif(VTK_FOUND)

set(SRC_FILES
    polyObject.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(polyobject ${SRC_FILES})
target_link_libraries(polyobject ${QT_LIBRARIES} vtkIO)
