find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Threads REQUIRED)

# Note set CUDA_BUILD_EMULATION to OFF to enable Device mode compilation.
#set(CUDA_SDK /usr/local/src/NVIDIA_CUDA_SDK)
#set(CUDA_INC ${CUDA_SDK}/common/inc/)

find_package(CUDA QUIET REQUIRED) #Any version OK
# find_package(CUDA 4.2 EXACT QUIET REQUIRED)

if (CUDA_FOUND)
   message("CUDA found")
else()
   message("CUDA not found")
endif()

cuda_include_directories(${CUDA_INC} ${CMAKE_CURRENT_SOURCE_DIR})
set(CUDA_HOST_COMPILER /usr/bin/gcc-4.9)

link_directories(${CUDA_SDK}/lib/)

set(HDRS book.h cpu_anim.h cpu_bitmap.h gl_helper.h gpu_anim.h)

set(EASYPROGS hello_world 
  simple_kernel simple_kernel_params simple_device_call
  enum_gpu set_gpu add_loop_gpu add_loop_long
)

#CUDA!
foreach(PROG ${EASYPROGS})
 cuda_add_executable(${PROG} ${PROG}.cu ${HDRS} )
 target_link_libraries(${PROG} ${CMAKE_THREAD_LIBS_INIT})
endforeach()

set(CGLLIBS ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} m)

#CS35 stuff. with CMake
add_executable(add_loop_cpu add_loop_cpu.cpp timer.cpp timer.h)

#CS40 stuff. link against opengl
add_executable(julia_cpu julia_cpu.cpp timer.h timer.cpp cpu_bitmap.h)
target_link_libraries(julia_cpu ${CGLLIBS})

cuda_add_executable(julia_gpu_timed julia_gpu_timed.cu ${HDRS})
target_link_libraries(julia_gpu_timed ${CGLLIBS})

cuda_add_executable(julia_gpu julia_gpu.cu ${HDRS})
target_link_libraries(julia_gpu ${CGLLIBS})


# Add a special target to clean nvcc generated files.
cuda_build_clean_target()
