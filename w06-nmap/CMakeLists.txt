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
  vshader2.glsl
  fshader2.glsl
)

ADD_EXECUTABLE(nmap ${SRC} ${SHDRS} ${MYQTFILES} ${SHADERS})
qt5_use_modules(nmap OpenGL Widgets)

TARGET_LINK_LIBRARIES(nmap ${EXT_LIBS} libcommon)

symlink_data(nmap_shaders *.glsl)
symlink_data(nmap_data data)


