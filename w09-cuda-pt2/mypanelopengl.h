#pragma once

/* Needed to get some OpenGL function
 * names through QT5. Oh, QT5. */
#define GL_GLEXT_PROTOTYPES

#include "common/sphere.h"
#include "common/square.h"
#include "cudahelpers.h"
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QtOpenGL>

typedef QVector4D point4;
typedef QVector4D color4;
typedef QVector3D vec3;
typedef QMatrix4x4 mat4;

#define CS40_NUM_PROGS 1
class MyPanelOpenGL : public QOpenGLWidget {
  Q_OBJECT

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void keyPressEvent(QKeyEvent *event);

public:
  explicit MyPanelOpenGL(QWidget *parent = 0);
  virtual ~MyPanelOpenGL();

private:
  /* simple test shapes */
  cs40::Sphere *m_sphere;
  cs40::Square *m_square;

  int m_polymode;

  bool m_drawSphere;

  QOpenGLTexture *m_texture;
  QOpenGLTexture *m_texture2;
  int m_tex_map;

  QOpenGLBuffer *m_pbo; /* Pixel Buffer Object */
  int m_pboSize;
  MyCUDAWrapper m_wrapper;

  float m_real, m_imaginary;

  vec3 m_angles; /* Euler angles for rotation */

  mat4 m_model;
  mat4 m_camera;
  mat4 m_projection;

  /* Shaders and programs */
  QOpenGLShader *m_vertexShaders[CS40_NUM_PROGS];
  QOpenGLShader *m_fragmentShaders[CS40_NUM_PROGS];
  QOpenGLShaderProgram *m_shaderPrograms[CS40_NUM_PROGS];
  int m_curr_prog; // current program ID

  // Make sure texture is set based on m_tex_map
  void setTexture();

  void textureReload(); // run kernel, write texture from PBO

  /* update Euler angle at index idx by amt
   * idx=0,1,2 -> x,y,z */
  void updateAngles(int idx, qreal amt);

  /* update model matrix based on angle */
  void updateModel();

  /* wrap a angle value to the range 0..360*/
  qreal wrap(qreal amt);

  /* update Polygon draw mode based
   * 0 : point
   * 1 : line
   * 2 : polygon */
  void updatePolyMode(int val);

  void createShaders(int i, QString vertName, QString fragName);
  void destroyShaders(int i);

  void createPBO();
  void destroyPBO();

signals:

public slots:
};
