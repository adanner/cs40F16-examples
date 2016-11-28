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
  vfraglight.glsl
  ffraglight.glsl
  vnormals.glsl
  fnormals.glsl
  gnormals.glsl
)

ADD_EXECUTABLE(gshader ${SRC} ${SHDRS} ${MYQTFILES} ${SHADERS})
qt5_use_modules(gshader OpenGL Widgets)

TARGET_LINK_LIBRARIES(gshader ${EXT_LIBS} libcommon)

symlink_data(gshaders *.glsl)


