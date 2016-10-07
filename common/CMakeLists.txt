INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} .)

set(SHDRS sphere.h square.h cylinder.h matrixstack.h)
set(SSRC sphere.cpp square.cpp cylinder.cpp matrixstack.cpp)

add_library(libcommon ${SHDRS} ${SSRC})
