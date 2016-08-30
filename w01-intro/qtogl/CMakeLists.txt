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

# Allow shaders to show up in QT Creator
set(SHADERS
  shaders/vshader.glsl
  shaders/fshader.glsl
)

# main executable
ADD_EXECUTABLE(qtogl ${SRC} ${MYQTFILES} ${SHADERS})

TARGET_LINK_LIBRARIES(qtogl ${EXT_LIBS} Qt5::Widgets Qt5::OpenGL)
#qt5_use_modules(qtogl OpenGL Widgets)

#link shaders folder to build directory
symlink_data(qtogl_shaders shaders)


