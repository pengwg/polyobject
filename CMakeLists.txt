project(RthReconCustomScriptPlugin)

cmake_minimum_required(VERSION 2.6.0)


find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

find_package(RthLibs REQUIRED)


# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

set(QT_USE_QTSCRIPT TRUE)
set(CMAKE_BUILD_TYPE Debug)

include(${QT_USE_FILE})

#############
# Find QWT5
#############
set(TEMP_PATH ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qwt5 REQUIRED)
set(CMAKE_MODULE_PATH ${TEMP_PATH})


# the variable "qtproject_SRCS" contains all .cpp files of this project
set(RthReconCustomScriptPlugin_SRCS
	RthReconCustomScriptPlugin.cpp
	RthMultiCoilPlot.cpp
	CoilImagePlot.cpp
)

set(RthReconCustomScriptPlugin_MOC_HDRS
	RthReconCustomScriptPlugin.h
	RthMultiCoilPlot.h
	CoilImagePlot.h
)

set(UI_FILES
    PlotControl.ui
)

include_directories(${QT_INCLUDES})
include_directories(${Qwt5_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${RTHLIBS_INCLUDE_DIR})

QT4_WRAP_UI(UI_FILES_OUT ${UI_FILES})
add_custom_target(UIFirst DEPENDS ${UI_FILES_OUT})

QT4_WRAP_CPP(RthReconCustomScriptPlugin_MOC_SRCS ${RthReconCustomScriptPlugin_MOC_HDRS})


# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_SHARED)
# ADD_DEFINITIONS(-DQT_NO_DEBUG)

ADD_LIBRARY(RthMultiCoilPlotPlugin MODULE ${RthReconCustomScriptPlugin_SRCS} ${RthReconCustomScriptPlugin_MOC_SRCS})

target_link_libraries(RthMultiCoilPlotPlugin ${QT_LIBRARIES} ${RTHLIBS_LIBRARIES})

add_dependencies(RthMultiCoilPlotPlugin UIFirst)

INSTALL(TARGETS RthMultiCoilPlotPlugin LIBRARY DESTINATION lib/RthScriptPlugins/script)
