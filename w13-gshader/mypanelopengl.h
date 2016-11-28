#pragma once

#include "common/sphere.h"
#include "common/square.h"
#include <QMatrix4x4>
#include <QtOpenGL>
#include <QtWidgets>

#define NUMPROGS 3

class MyPanelOpenGL : public QOpenGLWidget {
  typedef QVector4D point4;
  typedef QVector4D color4;
  typedef QVector3D vec3;
  typedef QMatrix4x4 mat4;

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
  bool m_cull;

  vec3 m_angles; /* Euler angles for rotation */

  mat4 m_model;
  mat4 m_camera;
  mat4 m_projection;

  /* Shaders and program */
  QOpenGLShader *m_vertexShaders[NUMPROGS];
  QOpenGLShader *m_fragmentShaders[NUMPROGS];
  QOpenGLShader *m_geometryShaders[NUMPROGS];
  QOpenGLShaderProgram *m_shaderPrograms[NUMPROGS];
  int m_curr_prog; // current program ID

  /* draw square originally in z=0 plane, rotated about y-axis
   * by yangle (in degrees) */
  void drawSquare(float yangle, int prog,
			bool withPoints=false);

  /* update Euler angle at index idx by amt
   * idx=0,1,2 -> x,y,z */
  void updateAngles(int idx, qreal amt);

  /* wrap a angle value to the range 0..360*/
  qreal wrap(qreal amt);

  /* update Polygon draw mode based
   * 0 : point
   * 1 : line
   * 2 : polygon */
  void updatePolyMode(int val);

	void drawShapes(int prog);
	void drawNormals();

  void createShaders(int i, 
			QString vertName, QString fragName, QString geomName);
  void destroyShaders(int i);

signals:

public slots:
};
