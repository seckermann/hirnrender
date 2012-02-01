PROJECT(isis_tutorial)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

###########################################################
# find and configure the isis core library
###########################################################
FIND_PACKAGE(ISIS REQUIRED)
INCLUDE_DIRECTORIES(${ISIS_INCLUDE_DIRS})

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

###############################################################################
# add global options
###############################################################################
ADD_DEFINITIONS(-D_ENABLE_LOG=1)
ADD_DEFINITIONS(-D_ENABLE_DEBUG=1)

###############################################################################
# add a target
###############################################################################
add_executable(renderer src/renderer.cpp src/volume.cpp src/volume.h src/vtkSliderCallback.cpp src/vtkSliderCallback.h)
target_link_libraries(renderer ${ISIS_LIB} ${ISIS_LIB_DEPENDS} ${VTK_LIBRARIES})

#SET(QT_USE_QTOPENGL TRUE)
#SET(qt_SOURCES  src/t.cpp src/window.cpp src/slidersgroup.cpp)
#SET(qt_moc_SOURCES src/slidersgroup.h src/window.h)
#QT4_WRAP_CPP(qt_SOURCES ${qt_moc_SOURCES})
#add_executable(qt ${qt_SOURCES})
#target_link_libraries(qt ${QT_LIBRARIES})

