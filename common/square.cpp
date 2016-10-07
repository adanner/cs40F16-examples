#include "square.h"
#include <iostream>

using namespace cs40;
using std::cout;
using std::endl;

Square::Square(float width)
    : m_color(1, 0, 0, 1), m_spec_color(1, 1, 1, 1), m_vbo(NULL),
      m_width(width) {
  vec3 *vertices = new vec3[4];
  vec2 *texCoords = new vec2[4];
  vec3 *normals = new vec3[4];
  vec3 *tangents = new vec3[4];
  float d = width / 2;
  vertices[0] = vec3(-d, -d, 0);
  texCoords[0] = vec2(0, 0);
  normals[0] = vec3(0, 0, 1.);
  tangents[0] = vec3(1., 0, 0.);
  vertices[1] = vec3(d, -d, 0);
  texCoords[1] = vec2(1, 0);
  normals[1] = vec3(0, 0, 1.);
  tangents[1] = vec3(1., 0, 0.);
  vertices[2] = vec3(-d, d, 0);
  texCoords[2] = vec2(0, 1);
  normals[2] = vec3(0, 0, 1.);
  tangents[2] = vec3(1., 0, 0.);
  vertices[3] = vec3(d, d, 0);
  texCoords[3] = vec2(1, 1);
  normals[3] = vec3(0, 0, 1.);
  tangents[3] = vec3(1., 0, 0.);

  if (initVBO()) {
    m_vbo->bind();
    m_vbo->allocate(4 * (3*sizeof(vec3) + sizeof(vec2)));
    m_vbo->write(0, vertices, 4 * sizeof(vec3));
    m_vbo->write(4 * sizeof(vec3), texCoords, 4 * sizeof(vec2));
    m_vbo->write(4 * (sizeof(vec3) + sizeof(vec2)), normals, 4 * sizeof(vec3));
    m_vbo->write(4 * (2*sizeof(vec3) + sizeof(vec2)), tangents, 4 * sizeof(vec3));
    m_vbo->release();
  }

  delete[] vertices;
  vertices = NULL;
  delete[] texCoords;
  texCoords = NULL;
  delete[] normals;
  normals = NULL;
  delete[] tangents;
  tangents = NULL;
}

bool Square::initVBO() {
  m_vao = new QOpenGLVertexArrayObject();
  bool ok = m_vao->create();
  if (!ok) {
    return false;
  }
  m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  ok = m_vbo->create();
  if (!ok) {
    return false;
  }
  m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
  return ok;
}

Square::~Square() {
  if (m_vbo) {
    m_vbo->release();
    delete m_vbo;
    m_vbo = NULL;
  }
  if (m_vao) {
    m_vao->release();
    delete m_vao;
    m_vao = NULL;
  }
}

void Square::draw(QOpenGLShaderProgram *prog) {
  if (!prog) {
    return;
  }
  m_vao->bind();
  m_vbo->bind();
  prog->setUniformValue("vColor", m_color);
  prog->setUniformValue("vSColor", m_spec_color);
  prog->enableAttributeArray("vPosition");
  prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 3, 0);
  prog->enableAttributeArray("vTexture");
  prog->setAttributeBuffer("vTexture", GL_FLOAT, 4 * sizeof(vec3), 2, 0);
  prog->enableAttributeArray("vNormal");
  prog->setAttributeBuffer("vNormal", GL_FLOAT,
                           4 * (sizeof(vec3) + sizeof(vec2)), 3, 0);
  prog->enableAttributeArray("vTangent");
  prog->setAttributeBuffer("vTangent", GL_FLOAT,
                           4 * (2*sizeof(vec3) + sizeof(vec2)), 3, 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  m_vbo->release();
  m_vao->release();
}
