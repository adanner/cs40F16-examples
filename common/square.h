#pragma once

#include <QtCore>
#include <QtOpenGL>
#include <cmath>

namespace cs40 {

class Square {
  typedef QVector2D vec2;
  typedef QVector3D vec3;
  typedef QVector4D vec4;

public:
  /* construct a square with given width
               * centered at origin, in z=0 plane*/
  Square(float width);

  ~Square();

  /* draw the square using provided shader program  */
  void draw(QOpenGLShaderProgram *prog);

  /* Set ambient and diffuse color of Square */
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
  QOpenGLVertexArrayObject *m_vao;
  QOpenGLBuffer *m_vbo;
  float m_width;
};

} // namespace
