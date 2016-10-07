#include "sphere.h"
#include <iostream>

using namespace cs40;
using std::cout;
using std::endl;

Sphere::Sphere(float radius, int slices, int stacks)
    : m_color(0, 0, 1, 1), m_spec_color(1, 1, 1, 1), m_vbo(NULL),
      m_radius(radius), m_slices(slices), m_stacks(stacks) {

  m_stripsize = (slices + 1) * 2;
  int nverts = m_stripsize * (m_stacks - 2) + 2 * (slices + 2);
  vec3 *vertices = new vec3[nverts];
  vec3 *normals = new vec3[nverts];
  vec3 *tangents = new vec3[nverts];
  vec2 *texCoords = new vec2[nverts];
  double latstep = M_PI / m_stacks;
  double longstep = 2. * M_PI / m_slices;
  double texXstep = 1. / m_slices;
  double texYstep = 1. / m_stacks;

  double lat0 = -M_PI / 2. + latstep;
  double z0 = m_radius * sin(lat0);
  double zcos0 = m_radius * cos(lat0);
  double texY = texYstep;

  double lng = M_PI;
  double texX = 0;
  double coslong = cos(lng);
  double sinlong = sin(lng);
  int idx = 0;

  /* mid lat strips */
  for (int i = 1; i < m_stacks - 1; i++) {

    double lat1 = lat0 + latstep;
    double z1 = m_radius * sin(lat1);
    double zcos1 = m_radius * cos(lat1);

    // cout << 180*lat0/M_PI << " " << 180*lat1/M_PI << endl;
    lng = -M_PI;
    texX = 0;

    for (int j = 0; j <= m_slices; j++) {
      coslong = cos(lng);
      sinlong = sin(lng);
      vertices[idx] = vec3(zcos1 * coslong, zcos1 * sinlong, z1);
      normals[idx] = vec3(cos(lat1) * coslong, cos(lat1) * sinlong, sin(lat1));
      tangents[idx] = vec3(-sinlong, coslong, 0.);
      texCoords[idx++] = vec2(texX, texY + texYstep);
      vertices[idx] = vec3(zcos0 * coslong, zcos0 * sinlong, z0);
      normals[idx] = vec3(cos(lat0) * coslong, cos(lat0) * sinlong, sin(lat0));
      tangents[idx] = vec3(-sinlong, coslong, 0.);
      texCoords[idx++] = vec2(texX, texY);
      lng += longstep;
      texX += texXstep;
    }
    // swap lat1, z1, zcos1 up
    lat0 = lat1;
    z0 = z1;
    zcos0 = zcos1;
    texY += texYstep;
  }

  /* draw polar caps as fans */

  /* north pole */
  vertices[idx] = vec3(0, 0, m_radius);
  normals[idx] = vec3(0, 0., 1.);
  tangents[idx] = vec3(1., 0., 0.);
  texCoords[idx++] = vec2(0.5, 1);
  lat0 = M_PI / 2 - latstep;
  lng = -M_PI;
  z0 = m_radius * sin(lat0);
  texX = 0;
  texY = 1 - texYstep;
  zcos0 = m_radius * cos(lat0);
  for (int i = 0; i <= m_slices; i++) {
    coslong = cos(lng);
    sinlong = sin(lng);
    vertices[idx] = vec3(zcos0 * coslong, zcos0 * sinlong, z0);
    normals[idx] = vec3(cos(lat0) * coslong, cos(lat0) * sinlong, sin(lat0));
    tangents[idx] = vec3(-sinlong, coslong, 0.);
    texCoords[idx++] = vec2(texX, texY);
    lng += longstep;
    texX += texXstep;
  }

  /* south pole */
  vertices[idx] = vec3(0, 0, -m_radius);
  normals[idx] = vec3(0, 0., -1.);
  tangents[idx] = vec3(-1, 0., 0.);
  texCoords[idx++] = vec2(0.5, 0);
  lat0 = -M_PI / 2 + latstep;
  lng = M_PI; /* Q: why M_PI and not -M_PI */
  texX = 1;
  texY = texYstep;
  z0 = m_radius * sin(lat0);
  zcos0 = m_radius * cos(lat0);
  for (int i = 0; i <= m_slices; i++) {
    coslong = cos(lng);
    sinlong = sin(lng);
    vertices[idx] = vec3(zcos0 * coslong, zcos0 * sinlong, z0);
    normals[idx] = vec3(cos(lat0) * coslong, cos(lat0) * sinlong, sin(lat0));
    tangents[idx] = vec3(-sinlong, coslong, 0);
    texCoords[idx++] = vec2(texX, texY);
    lng -= longstep; /* Q: why -= ?*/
    texX -= texXstep;
  }

  if (initVBO()) {
    int DataSize =
        m_stripsize * (m_stacks - 2) * sizeof(vec3); /* all mid lat strips */
    DataSize += 2 * (m_slices + 2) * sizeof(vec3);   /* two polar fans*/
    /* size of Texture */
    int TexSize =
        m_stripsize * (m_stacks - 2) * sizeof(vec2); /* all mid lat strips */
    TexSize += 2 * (m_slices + 2) * sizeof(vec2);    /* two polar fans*/
    /* size of Normals, Tangents */
    int NormalSize = DataSize;
    // cout << "DataSize: " << DataSize + TexSize << endl;
    m_vbo->bind();
    m_vbo->allocate(DataSize + TexSize + 2*NormalSize);
    m_vbo->write(0, vertices, DataSize);
    m_vbo->write(DataSize, texCoords, TexSize);
    m_vbo->write(DataSize + TexSize, normals, NormalSize);
    m_vbo->write(DataSize + TexSize + NormalSize, tangents, NormalSize);
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

bool Sphere::initVBO() {
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

Sphere::~Sphere() {
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

void Sphere::draw(QOpenGLShaderProgram *prog) {
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
  int nverts = (m_stacks - 2) * m_stripsize + 2 * (m_slices + 2);
  prog->setAttributeBuffer("vTexture", GL_FLOAT, nverts * sizeof(vec3), 2, 0);
  prog->enableAttributeArray("vNormal");
  prog->setAttributeBuffer("vNormal", GL_FLOAT,
                           nverts * (sizeof(vec3) + sizeof(vec2)), 3, 0);
  prog->enableAttributeArray("vTangent");
  prog->setAttributeBuffer("vTangent", GL_FLOAT,
                           nverts * (2*sizeof(vec3) + sizeof(vec2)), 3, 0);
  for (int i = 0; i < m_stacks - 2; i++) {
    glDrawArrays(GL_TRIANGLE_STRIP, i * m_stripsize, m_stripsize);
  }
  int offset = (m_stacks - 2) * m_stripsize;
  int fansize = m_slices + 2;
  glDrawArrays(GL_TRIANGLE_FAN, offset, fansize);
  glDrawArrays(GL_TRIANGLE_FAN, offset + fansize, fansize);
  m_vbo->release();
  m_vao->release();
}
