INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} .)

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

set(SHDRS sphere.h square.h matrixstack.h)
set(SSRC sphere.cpp square.cpp matrixstack.cpp)
add_library(libsphere5 ${SHDRS} ${SSRC})

set(SPLIBS ${EXT_LIBS} Qt5::Widgets Qt5::OpenGL)
ADD_EXECUTABLE(perspective ${SRC} ${SHDRS} ${MYQTFILES} ${SHADERS})

TARGET_LINK_LIBRARIES(perspective ${SPLIBS} libsphere5)

symlink_data(w05-shaders *.glsl)
symlink_data(w05-png data)


