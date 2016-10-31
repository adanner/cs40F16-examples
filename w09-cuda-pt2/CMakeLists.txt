find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Threads REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)

# Note set CUDA_BUILD_EMULATION to OFF to enable Device mode compilation.
#set(CUDA_SDK /usr/local/src/NVIDIA_CUDA_SDK)
#set(CUDA_INC ${CUDA_SDK}/common/inc/)

find_package(CUDA QUIET REQUIRED) #Any version OK

if (CUDA_FOUND)
   message("CUDA found")
else()
   message("CUDA not found")
endif()

cuda_include_directories(${CUDA_INC} ${CMAKE_CURRENT_SOURCE_DIR})
set(CUDA_HOST_COMPILER /usr/bin/gcc-4.9)
link_directories(${CUDA_SDK}/lib/)

set(HDRS book.h cpu_anim.h cpu_bitmap.h 
  gl_helper.h gpu_anim.h
  timer.h)

cuda_add_library(mycuda 
  timer.h book.h cudahelpers.h
  timer.cu book.cu cudahelpers.cu)


set(GPROG dot)

foreach(PROG ${GPROG})
  cuda_add_executable(${PROG} ${PROG}.cu ${HDRS})
  target_link_libraries(${PROG} 
    ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} m mycuda)
endforeach()

#QT headers requiring MOC (meta object compiler)
set(QTHDRS 
  mainwindow.h 
  mypanelopengl.h
  )

#window design files
set(QTFORMS mainwindow.ui)

#Run qt tools to auto generate C++ compatible files
QT5_WRAP_CPP(QTHDRS_MOC ${QTHDRS})
QT5_WRAP_UI(QTFORMS_HDRS ${QTFORMS})
SET(MYQTFILES ${QTHDRS_MOC} ${QTFORMS_HDRS})

#normal source files
set(SRC 
  main.cpp 
  mainwindow.cpp 
  mypanelopengl.cpp
)

set(SHADERS
  vshader.glsl
  fshader.glsl
)

ADD_EXECUTABLE(interop ${SRC} ${SHDRS} ${MYQTFILES} ${SHADERS})

TARGET_LINK_LIBRARIES(interop ${EXT_LIBS} libcommon mycuda
  Qt5::Core Qt5::Gui Qt5::OpenGL)

symlink_data(interop_shaders *.glsl)

