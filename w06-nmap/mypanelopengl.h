#pragma once

#include "common/sphere.h"
#include "common/square.h"
#include <QMatrix4x4>
#include <QtOpenGL>
#include <QtWidgets>

typedef QVector4D point4;
typedef QVector4D color4;
typedef QVector3D vec3;
typedef QMatrix4x4 mat4;

#define CS40_NUM_PROGS 3

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
  int m_normal_map;
  bool m_drawSphere;
  bool m_cull;

  QOpenGLTexture *m_texture;
  QOpenGLTexture *m_normalMap1;
  QOpenGLTexture *m_normalMap2;

  vec3 m_angles; /* Euler angles for rotation */

  mat4 m_model;
  mat4 m_camera;
  mat4 m_projection;

  /* Shaders and programs */
  QOpenGLShader *m_vertexShaders[CS40_NUM_PROGS];
  QOpenGLShader *m_fragmentShaders[CS40_NUM_PROGS];
  QOpenGLShaderProgram *m_shaderPrograms[CS40_NUM_PROGS];
  int m_curr_prog; // current program ID

  void showUsage();

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

signals:

public slots:
};
