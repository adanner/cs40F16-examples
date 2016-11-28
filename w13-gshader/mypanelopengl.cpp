#include "mypanelopengl.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;
using cs40::Sphere;
using cs40::Square;
using cs40::vec4;

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent)
    : QOpenGLWidget(parent), m_angles(0, 15., 0.) {

  for (int i = 0; i < NUMPROGS; i++) {
    m_shaderPrograms[i] = NULL;
    m_vertexShaders[i] = NULL;
    m_fragmentShaders[i] = NULL;
    m_geometryShaders[i] = NULL;
  }

  m_sphere = NULL;
  m_square = NULL;
  m_drawSphere = true;
  m_polymode = 2;
  m_cull = true;
}

MyPanelOpenGL::~MyPanelOpenGL() {
  delete m_sphere;
  m_sphere = NULL;
  delete m_square;
  m_square = NULL;
  destroyShaders(0);
  destroyShaders(1);
}

void MyPanelOpenGL::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  updatePolyMode(m_polymode);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  createShaders(0, "vfraglight.glsl", "ffraglight.glsl", "");
  createShaders(1, "vnormals.glsl", "fnormals.glsl", "gnormals.glsl");

  m_sphere = new Sphere(0.5, 10, 10);
  m_square = new Square(1.);

  m_projection.perspective(40, 1, 1, 8);
  m_camera.lookAt(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1., 0.));
  m_model.setToIdentity();
}

void MyPanelOpenGL::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void MyPanelOpenGL::paintGL() {

  setModes(); /* clear, set poly mode, culling */

  drawScene(SHAPE_PROG);
  if (m_drawNormals) {
    drawScene(NORMAL_PROG);
  }
  glFlush();
}

void MyPanelOpenGL::setModes() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  updatePolyMode(m_polymode);
  if (m_cull) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

void MyPanelOpenGL::setUniforms(int prog) {
  m_shaderPrograms[prog]->bind();
  m_shaderPrograms[prog]->setUniformValue("projection", m_projection);
  m_shaderPrograms[prog]->setUniformValue("camera", m_camera);
  m_shaderPrograms[prog]->setUniformValue(
      "lightPos", vec4(1.0, 0, 2, 1.)); // in world coordinates
  updateModel(prog);
}

void MyPanelOpenGL::updateModel(int prog) {
  m_shaderPrograms[prog]->setUniformValue("model", m_model);
  mat4 mview = m_camera * m_model;
  m_shaderPrograms[prog]->setUniformValue("modelView", mview);
  m_shaderPrograms[prog]->setUniformValue("normalMatrix", mview.normalMatrix());
}

void MyPanelOpenGL::drawScene(int prog) {

  if (!m_shaderPrograms[prog]) {
    return;
  }
  setUniforms(prog);

  /* draw geometry as points? */
  bool withPoints = (prog == NORMAL_PROG);

  m_stack.push();
  /*global transforms */

  if (m_drawSphere) {
    m_stack.push();
    /*sphere transforms */
    m_model = m_stack.top();
    updateModel(prog);
    m_sphere->draw(m_shaderPrograms[prog], withPoints);
    m_stack.pop();
  }

  m_stack.push();
  /*square transforms */
  m_stack.rotate(m_angles.y(), vec3(0, 1, 0));
  m_model = m_stack.top();
  updateModel(prog);
  m_square->draw(m_shaderPrograms[prog], withPoints);
  m_stack.pop();

  m_stack.pop();
}

void MyPanelOpenGL::drawSquare(float yangle, bool withPoints) {
  m_model.setToIdentity();
  m_model.rotate(yangle, vec3(0, 1, 0));
  mat4 mview = m_camera * m_model;
  int prog = withPoints ? NORMAL_PROG : SHAPE_PROG;
  m_shaderPrograms[prog]->setUniformValue("model", m_model);
  m_shaderPrograms[prog]->setUniformValue("modelView", mview);
  m_shaderPrograms[prog]->setUniformValue("normalMatrix", mview.normalMatrix());
  m_square->draw(m_shaderPrograms[prog], withPoints);
  m_model.setToIdentity();
}

void MyPanelOpenGL::keyPressEvent(QKeyEvent *event) {
  qreal step = 1;
  /*Enable strong Focus on GL Widget to process key events*/
  switch (event->key()) {
  case Qt::Key_X:
    if (event->text() == "x") {
      updateAngles(0, step);
    } else {
      updateAngles(0, -step);
    }
    break;
  case Qt::Key_Y:
    if (event->text() == "y") {
      updateAngles(1, step);
    } else {
      updateAngles(1, -step);
    }
    break;
  case Qt::Key_Z:
    if (event->text() == "z") {
      updateAngles(2, step);
    } else {
      updateAngles(2, -step);
    }
    break;
  case Qt::Key_C:
    m_cull = !m_cull;
    break;
  case Qt::Key_P:
    m_polymode = (m_polymode + 1) % 3;
    break;
  case Qt::Key_S:
    m_drawSphere = !m_drawSphere;
    break;
  case Qt::Key_N:
    m_drawNormals = !m_drawNormals;
    break;
  default:
    QWidget::keyPressEvent(event); /* pass to base class */
  }
  update();
}

void MyPanelOpenGL::updateAngles(int idx, qreal amt) {
  if (idx == 0) {
    m_angles.setX(m_angles.x() + amt);
  } else if (idx == 1) {
    m_angles.setY(m_angles.y() + amt);
  } else if (idx == 2) {
    m_angles.setZ(m_angles.z() + amt);
  }
}

qreal MyPanelOpenGL::wrap(qreal amt) {
  if (amt > 360.) {
    return amt - 360.;
  } else if (amt < 0.) {
    return amt + 360.;
  }
  return amt;
}

void MyPanelOpenGL::updatePolyMode(int val) {
  GLenum mode = GL_NONE;
  if (val == 0) {
    mode = GL_POINT;
  } else if (val == 1) {
    mode = GL_LINE;
  } else if (val == 2) {
    mode = GL_FILL;
  }

  if (mode != GL_NONE) {
    glPolygonMode(GL_FRONT_AND_BACK, mode);
  }
}

void MyPanelOpenGL::createShaders(int i, QString vertName, QString fragName,
                                  QString geomName) {

  cout << "building shader " << i << endl;
  destroyShaders(i); // get rid of any old shaders
  m_vertexShaders[i] = new QOpenGLShader(QOpenGLShader::Vertex);
  if (!m_vertexShaders[i]->compileSourceFile(vertName)) {
    qWarning() << m_vertexShaders[i]->log();
  }

  m_fragmentShaders[i] = new QOpenGLShader(QOpenGLShader::Fragment);
  if (!m_fragmentShaders[i]->compileSourceFile(fragName)) {
    qWarning() << m_fragmentShaders[i]->log();
  }

  if (geomName.length() > 1) {
    m_geometryShaders[i] = new QOpenGLShader(QOpenGLShader::Geometry);
    if (!m_geometryShaders[i]->compileSourceFile(geomName)) {
      qWarning() << m_geometryShaders[i]->log();
    }
  }

  m_shaderPrograms[i] = new QOpenGLShaderProgram();
  m_shaderPrograms[i]->addShader(m_vertexShaders[i]);
  m_shaderPrograms[i]->addShader(m_fragmentShaders[i]);
  if (m_geometryShaders[i] != NULL) {
    m_shaderPrograms[i]->addShader(m_geometryShaders[i]);
  }
  if (!m_shaderPrograms[i]->link()) {
    qWarning() << m_shaderPrograms[i]->log() << endl;
  }
}

void MyPanelOpenGL::destroyShaders(int i) {
  delete m_vertexShaders[i];
  m_vertexShaders[i] = NULL;
  delete m_fragmentShaders[i];
  m_fragmentShaders[i] = NULL;
  delete m_geometryShaders[i];
  m_geometryShaders[i] = NULL;

  if (m_shaderPrograms[i]) {
    m_shaderPrograms[i]->release();
    delete m_shaderPrograms[i];
    m_shaderPrograms[i] = NULL;
  }
}
