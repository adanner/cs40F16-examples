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
  vshader.glsl vshader2.glsl
  fshader.glsl fshader2.glsl
)

ADD_EXECUTABLE(particles ${SRC} ${SHDRS} ${MYQTFILES} ${SHADERS})

TARGET_LINK_LIBRARIES(particles ${EXT_LIBS} libcommon
  Qt5::Core Qt5::Gui Qt5::OpenGL)

symlink_data(particle_shaders *.glsl)
symlink_data(particle_data data)



