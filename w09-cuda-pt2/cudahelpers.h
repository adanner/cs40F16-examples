#include <GL/gl.h>
/* Wrapper class for connecting OpenGL context to CUDA */
class MyCUDAWrapper {

public:
  MyCUDAWrapper();
  ~MyCUDAWrapper(){};

  /* Call CUDA initializization during OpenGL init */
  void init();

  /* Connect Pixel Buffer Object with buffID to
   * CUDA resource */
  void connect(GLuint buffID);

  /* Release CUDA resource connected to Pixel Buffer*/
  void disconnect();

  /* Grab PBO for CUDA use, run kernel,
   * and return PBO to OpenGL context */
  void run(float a, float b);

private:
  struct cudaGraphicsResource *m_pbo_CUDA;
};
