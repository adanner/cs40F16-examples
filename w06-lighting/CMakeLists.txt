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
  vfraglight.glsl
  ffraglight.glsl
)

ADD_EXECUTABLE(phong ${SRC} ${SHDRS} ${MYQTFILES} ${SHADERS})
qt5_use_modules(phong OpenGL Widgets)

TARGET_LINK_LIBRARIES(phong ${EXT_LIBS} libcommon)

symlink_data(phong_shaders *.glsl)


