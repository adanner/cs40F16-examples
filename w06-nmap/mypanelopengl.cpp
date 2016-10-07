#include "mypanelopengl.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;
using cs40::Sphere;
using cs40::Square;

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent)
    : QOpenGLWidget(parent), m_angles(-125., 0., 0.) {

  for (int i = 0; i < CS40_NUM_PROGS; i++) {
    m_shaderPrograms[i] = NULL;
    m_vertexShaders[i] = NULL;
    m_fragmentShaders[i] = NULL;
  }

  m_sphere = NULL;
  m_drawSphere = true;
  m_cull = true;
  m_polymode = 2;
  m_curr_prog = 0;
  m_normal_map = 0;

  m_texture = m_normalMap1 = m_normalMap2 = NULL;
}

MyPanelOpenGL::~MyPanelOpenGL() {
  m_shaderPrograms[m_curr_prog]->release();
  delete m_sphere;
  m_sphere = NULL;
  delete m_square;
  m_square = NULL;
  destroyShaders(0);
  destroyShaders(1);
}

void MyPanelOpenGL::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  updatePolyMode(m_polymode);

  showUsage();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  createShaders(0, "vshader.glsl", "fshader.glsl");
  createShaders(1, "vshader2.glsl", "fshader2.glsl");
  createShaders(2, "vshader3.glsl", "fshader3.glsl");

  m_sphere = new Sphere(0.5, 30, 30);
  m_square = new Square(1.);
  m_texture = new QOpenGLTexture(QImage("data/earth.png").mirrored());
  m_normalMap1 = new QOpenGLTexture(QImage("data/beady.jpg").mirrored());
  m_normalMap2 = new QOpenGLTexture(QImage("data/crinkly.jpg").mirrored());

  glActiveTexture(GL_TEXTURE0);
  m_texture->bind();
  glActiveTexture(GL_TEXTURE1);
  m_normalMap1->bind();

  m_shaderPrograms[m_curr_prog]->bind();

  m_projection.perspective(40, 1, 1, 8);
  m_camera.lookAt(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1., 0.));
  updateModel();
}

void MyPanelOpenGL::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void MyPanelOpenGL::paintGL() {
  /* clear both color and depth buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  updatePolyMode(m_polymode);
  if (m_cull) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }

  glActiveTexture(GL_TEXTURE1);
  if (m_normal_map == 0) {
    m_normalMap1->bind();
  } else if (m_normal_map == 1) {
    m_normalMap2->bind();
  }

  if (!m_shaderPrograms[m_curr_prog]) {
    return;
  }
  m_shaderPrograms[m_curr_prog]->bind();
  mat4 mview = m_camera * m_model;
  m_shaderPrograms[m_curr_prog]->setUniformValue("projection", m_projection);
  m_shaderPrograms[m_curr_prog]->setUniformValue("camera", m_camera);
  m_shaderPrograms[m_curr_prog]->setUniformValue("model", m_model);
  m_shaderPrograms[m_curr_prog]->setUniformValue("modelView", mview);
  m_shaderPrograms[m_curr_prog]->setUniformValue("normalMatrix",
                                                 mview.normalMatrix());
  m_shaderPrograms[m_curr_prog]->setUniformValue("Tex0", 0);
  m_shaderPrograms[m_curr_prog]->setUniformValue("Tex1", 1);
  m_shaderPrograms[m_curr_prog]->setUniformValue(
      "lightPos", vec4(1.5, 0, 2, 1.)); // in world coordinates

  if (m_drawSphere) {
    m_sphere->draw(m_shaderPrograms[m_curr_prog]);
    // m_square->draw(m_shaderProgram);
  } else {
    m_square->draw(m_shaderPrograms[m_curr_prog]);
  }
  glFlush();
}

void MyPanelOpenGL::showUsage() {
  cout << "x/y/z: Rotate" << endl;
  cout << "n: Cycle normal maps" << endl;
  cout << "v: Cycle shader program" << endl;
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
  case Qt::Key_N:
    m_normal_map = (m_normal_map + 1) % 2;
    break;
  case Qt::Key_P:
    m_polymode = (m_polymode + 1) % 3;
    updatePolyMode(m_polymode);
    break;
  case Qt::Key_S:
    m_drawSphere = !m_drawSphere;
    break;
  case Qt::Key_V:
    m_curr_prog = (m_curr_prog + 1) % CS40_NUM_PROGS;
    break;
  default:
    QWidget::keyPressEvent(event); /* pass to base class */
  }
  update();
}

void MyPanelOpenGL::updateAngles(int idx, qreal amt) {
  if (idx == 0) {
    m_angles.setX(wrap(m_angles.x() + amt));
  } else if (idx == 1) {
    m_angles.setY(wrap(m_angles.y() + amt));
  } else if (idx == 2) {
    m_angles.setZ(wrap(m_angles.z() + amt));
  }
  updateModel();
}

void MyPanelOpenGL::updateModel() {
  m_model.setToIdentity();
  m_model.rotate(m_angles.z(), vec3(0, 0, 1.));
  m_model.rotate(m_angles.y(), vec3(0, 1, 0.));
  m_model.rotate(m_angles.x(), vec3(1, 0, 0.));
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

void MyPanelOpenGL::createShaders(int i, QString vertName, QString fragName) {

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

  m_shaderPrograms[i] = new QOpenGLShaderProgram();
  m_shaderPrograms[i]->addShader(m_vertexShaders[i]);
  m_shaderPrograms[i]->addShader(m_fragmentShaders[i]);

  if (!m_shaderPrograms[i]->link()) {
    qWarning() << m_shaderPrograms[i]->log() << endl;
  }
}

void MyPanelOpenGL::destroyShaders(int i) {
  delete m_vertexShaders[i];
  m_vertexShaders[i] = NULL;
  delete m_fragmentShaders[i];
  m_fragmentShaders[i] = NULL;

  if (m_shaderPrograms[i]) {
    m_shaderPrograms[i]->release();
    delete m_shaderPrograms[i];
    m_shaderPrograms[i] = NULL;
  }
}
