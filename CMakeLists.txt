cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_FLAGS "-g -Wall")

#for FindGLM.cmake, FindGLFW3.cmake, other helpers
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules) 

FIND_PACKAGE(OpenGL REQUIRED) #for graphics
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(GLFW3 REQUIRED)
FIND_PACKAGE(GLM REQUIRED)

#required CMake options for QT Apps
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#for window widgets
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS})

#default OpenGL libs
SET(EXT_LIBS ${OPENGL_LIBRARY} ${GLEW_LIBRARY})

#get symlink_data macro
include(${CMAKE_SOURCE_DIR}/cmake_modules/helpers.cmake)

INCLUDE_DIRECTORIES(
  ${GLEW_INCLUDE_DIR} 
  ${GLFW3_INCLUDE_DIR} 
  ${GLM_INCLUDE_DIR} 
  ./
  ) 
#things to actually build
add_subdirectory(w01-intro)
add_subdirectory(w02-opengl)
add_subdirectory(w04-3D)

