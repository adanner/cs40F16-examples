cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE( GLUT )
FIND_PACKAGE( OpenGL )

SET( PROGRAMS cones) 


FOREACH( program ${PROGRAMS} )
  ADD_EXECUTABLE( ${program} ${program}.cpp )
  TARGET_LINK_LIBRARIES( ${program} 
    ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
ENDFOREACH( program )
