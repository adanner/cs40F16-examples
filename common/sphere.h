#pragma once

#include <QtOpenGL>
#include <cmath>

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

namespace cs40 {

class Sphere {
public:
  /* construct a sphere centered at origin with given radius
               * by dividing it into vertical slices and horizontal stacks */
  Sphere(float radius, int slices, int stacks);

  ~Sphere();

  /* draw the sphere using provided shader program  */
  void draw(QOpenGLShaderProgram *prog);

  /* Set Ambient and Diffuse color of Sphere */
  inline void setColor(const vec3 &color) {
    m_color = color;
    m_color.setW(1.);
  }

  inline void setSpecularColor(const vec3 &color) {
    m_spec_color = color;
    m_spec_color.setW(1.);
  }

  /* Get current color of object */
  inline vec4 getAmbientAndDiffuseColor() const { return m_color; }

  inline vec4 getSpecularColor() const { return m_spec_color; }

private:
  /* Generate Vertex Buffer Objects, but do not bind/allocate.
   * Return true if generate was a success, false otherwise
   */
  bool initVBO();

protected:
  vec4 m_color;
  vec4 m_spec_color;
  QOpenGLBuffer *m_vbo;
  QOpenGLVertexArrayObject *m_vao;
  float m_radius;
  int m_slices;
  int m_stacks;
  int m_stripsize; // num vertices per horizontal strip;
};

} // namespace
