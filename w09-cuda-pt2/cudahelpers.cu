#include "cudahelpers.h"
#include <cuda.h>
#include <cuda_gl_interop.h>
#include <cstdio>

#define DIM 1000
__device__ int julia( int x, int y, float re, float im);
__global__ void kernel( unsigned char *ptr, float re, float im);

MyCUDAWrapper::MyCUDAWrapper():m_pbo_CUDA(NULL){};

void MyCUDAWrapper::init(){
    cudaGLSetGLDevice(0);
}

void MyCUDAWrapper::connect(GLuint buffID){
    if(m_pbo_CUDA){ disconnect(); }
    cudaGraphicsGLRegisterBuffer(&m_pbo_CUDA,buffID,cudaGraphicsRegisterFlagsNone);
}

void MyCUDAWrapper::disconnect(){
    if(m_pbo_CUDA){
        cudaGraphicsUnregisterResource(m_pbo_CUDA);
    }
}

void MyCUDAWrapper::run(float a, float b){

    unsigned char* dev_pixBuffer;
    size_t numBytes;
    dim3    grid(DIM,DIM);

    // Map buffer object for writing from CUDA
    cudaGraphicsMapResources(1, &m_pbo_CUDA);
    cudaGraphicsResourceGetMappedPointer((void**)&dev_pixBuffer,
                                         &numBytes,
                                         m_pbo_CUDA);

    kernel<<<grid,1>>>( dev_pixBuffer, a, b);
    cudaThreadSynchronize(); //Make sure kernel is done
    //Return PBO to OpenGL control.
    cudaGraphicsUnmapResources(1, &m_pbo_CUDA);
}


struct cuComplex {
    float   r;
    float   i;
    __device__ cuComplex( float a, float b ) : r(a), i(b)  {}
    __device__ float magnitude2( void ) {
        return r * r + i * i;
    }
    __device__ cuComplex operator*(const cuComplex& a) {
        return cuComplex(r*a.r - i*a.i, i*a.r + r*a.i);
    }
    __device__ cuComplex operator+(const cuComplex& a) {
        return cuComplex(r+a.r, i+a.i);
    }
};

__device__ int julia( int x, int y, float re, float im) {
    const float scale = 1.5;
    float jx = scale * (float)(DIM/2 - x)/(DIM/2);
    float jy = scale * (float)(DIM/2 - y)/(DIM/2);

    cuComplex c(re, im);
    cuComplex a(jx, jy);

    int i = 0;
    for (i=0; i<200; i++) {
        a = a * a + c;
        if (a.magnitude2() > 1000)
            return 0;
    }

    return 1;
}

__global__ void kernel( unsigned char *ptr, float re, float im ) {
    // map from threadIdx/BlockIdx to pixel position
    int x = blockIdx.x;
    int y = blockIdx.y;
    int offset = x + y * gridDim.x;

    // now calculate the value at that position
    int juliaValue = julia( x, y, re, im);
    ptr[offset*4 + 0] = 255 * juliaValue;
    ptr[offset*4 + 1] = 0;
    ptr[offset*4 + 2] = 64*(1-juliaValue);
    ptr[offset*4 + 3] = 255;
}




