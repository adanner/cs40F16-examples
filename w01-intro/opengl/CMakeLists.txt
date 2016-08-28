set( chapter01_SOURCES  
	scenebasic.cpp
	main.cpp
	scenebasic_uniformblock.cpp
	scenebasic_attrib.cpp
	scenebasic_uniform.cpp
	scenebasic_debug.cpp )

set( cookbook_lib_SOURCES
  gl_core_4_4.c
  glutils.cpp
  glslprogram.cpp
)

add_library(cookbook_lib ${cookbook_lib_SOURCES})

add_executable( chapter01 ${chapter01_SOURCES} )
target_link_libraries( chapter01 cookbook_lib 
  ${GLFW3_LIBRARY} ${OPENGL_LIBRARY})

symlink_data(chapter01_shaders shader)
