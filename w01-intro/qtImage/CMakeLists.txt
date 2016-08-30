INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} .)

#QT headers requiring MOC (meta object compiler)
set(QTHDRS 
  mainwindow.h
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
)

ADD_EXECUTABLE(qtimgtest ${SRC} ${MYQTFILES})

TARGET_LINK_LIBRARIES(qtimgtest ${EXT_LIBS})
qt5_use_modules(qtimgtest OpenGL Widgets)

symlink_data(w01qtimage data)

