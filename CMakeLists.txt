PROJECT(isis_tutorial)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

###########################################################
# find and configure the isis core library
###########################################################
FIND_PACKAGE(ISIS REQUIRED)
INCLUDE_DIRECTORIES(${ISIS_INCLUDE_DIRS})

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
###############################################################################
# add global options
###############################################################################
ADD_DEFINITIONS(-D_ENABLE_LOG=1)
ADD_DEFINITIONS(-D_ENABLE_DEBUG=1)

###############################################################################
# add a target
###############################################################################
add_executable(renderer src/renderer.cpp)
target_link_libraries(renderer ${ISIS_LIB} ${ISIS_LIB_DEPENDS} ${VTK_LIBRARIES})
