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
  m_polymode = 2;
  m_curr_prog = 0;
  m_tex_map = 0;
  m_pbo = NULL;
  m_pboSize = 1000;
  m_real = -0.8;
  m_imaginary = 0.156;
  m_texture = m_texture2 = NULL;
}

MyPanelOpenGL::~MyPanelOpenGL() {
  m_shaderPrograms[m_curr_prog]->release();
  delete m_sphere;
  m_sphere = NULL;
  delete m_square;
  m_square = NULL;
  delete m_texture;
  m_texture = NULL;
  delete m_texture2;
  m_texture2 = NULL;
  destroyShaders(0);
  // Cleanup PBO/Texture
  m_wrapper.disconnect();
  destroyPBO();
}

void MyPanelOpenGL::initializeGL() {
  m_wrapper.init(); // Tell CUDA to connect with OpenGL

  // glewInit(); //manually do this now that we aren't using QtOpenGL
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  updatePolyMode(m_polymode);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  createShaders(0, "vshader.glsl", "fshader.glsl");

  m_texture = new QOpenGLTexture(QImage("data/earth.png").mirrored());
  m_texture2 = new QOpenGLTexture(QOpenGLTexture::Target2D);
  m_sphere = new Sphere(0.5, 30, 30);
  m_square = new Square(1.);

  m_shaderPrograms[m_curr_prog]->bind();

  m_projection.perspective(40, 1, 1, 8);
  m_camera.lookAt(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1., 0.));
  updateModel();

  createPBO(); // Setup Pixel Buffer on GPU
}

void MyPanelOpenGL::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void MyPanelOpenGL::paintGL() {
  /* clear both color and depth buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setTexture();
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

  // swapBuffers(); /* not need in QT see QGLWidget::setAutoBufferSwap */
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
    if (glIsEnabled(GL_CULL_FACE)) {
      glDisable(GL_CULL_FACE);
    } else {
      glEnable(GL_CULL_FACE);
    }
    break;
  case Qt::Key_P:
    m_polymode = (m_polymode + 1) % 3;
    updatePolyMode(m_polymode);
    break;
  case Qt::Key_S:
    m_drawSphere = !m_drawSphere;
    break;
  case Qt::Key_T:
    m_tex_map = (m_tex_map + 1) % 2;
    setTexture();
    break;
  case Qt::Key_V:
    m_curr_prog = (m_curr_prog + 1) % CS40_NUM_PROGS;
    break;
  case Qt::Key_R:
    if (event->text() == "r") {
      m_real += 0.01;
    } else {
      m_real -= 0.01;
    }
    textureReload();
    break;
  case Qt::Key_I:
    if (event->text() == "i") {
      m_imaginary += 0.01;
    } else {
      m_imaginary -= 0.01;
    }
    textureReload();
    break;
  default:
    QWidget::keyPressEvent(event); /* pass to base class */
  }
  update();
}

void MyPanelOpenGL::setTexture() {
  if (m_tex_map == 0) {
    m_texture->bind();
  } else if (m_tex_map == 1) {
    m_texture2->bind();
  }
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
  // glPolygonMode(GL_BACK,GL_POINT);
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

void MyPanelOpenGL::createPBO() {
  destroyPBO(); // get rid of any old buffer

  // Create PBO
  int numBytes = sizeof(GLubyte) * 4 * m_pboSize * m_pboSize;
  m_pbo = new QOpenGLBuffer(
      QOpenGLBuffer::PixelUnpackBuffer); // Used for reading Texture data
  m_pbo->create();
  m_pbo->bind();
  m_pbo->allocate(numBytes);
  m_wrapper.connect(m_pbo->bufferId()); // Inform CUDA about PBO

  // Create ID, allocate space for Texture
  m_texture2->create();
  m_texture2->bind();

  // Allocate the texture memory. The last parameter is NULL since we only
  // want to allocate memory, not initialize it
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pboSize, m_pboSize, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);

  // int err = glGetError();
  // cout << err << " " << glewGetErrorString(glGetError()) << endl;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // These last two are critical for textures whose dimension are not a power of
  // 2
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  textureReload();
}

void MyPanelOpenGL::textureReload() {
  // Run CUDA kernel to populate PBO
  m_wrapper.run(m_real, m_imaginary);
  // Read Texture data from PBO
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo->bufferId());
  m_texture2->bind();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_pboSize, m_pboSize, GL_RGBA,
                  GL_UNSIGNED_BYTE, NULL);
}

void MyPanelOpenGL::destroyPBO() {
  if (m_pbo) {
    m_pbo->release();
    delete m_pbo;
    m_pbo = NULL;
  }
}
