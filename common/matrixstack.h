#pragma once

#include <QMatrix4x4>
#include <QVector3D>

namespace cs40 {

class MatrixStack {
public:
  /* construct a stack containing identity matrix */
  MatrixStack();

  QMatrix4x4 &top();

  inline int size() { return m_stack.size(); }

  /* push copy of top onto stack */
  void push();

  /* pop top element off stack */
  void pop();

  /* translate matrix at top of stack in place
   * all matrix tranforms multiply on the right
   * of top */
  inline void translate(float dx, float dy, float dz = 0.) {
    top().translate(dx, dy, dz);
  }

  inline void translate(const QVector3D &displace) {
    top().translate(displace);
  }

  /* scales matrix at top of stack in place */
  inline void scale(float xfac, float yfac = 1., float zfac = 1.) {
    top().scale(xfac, yfac, zfac);
  }

  inline void scale(const QVector3D &vec) { top().scale(vec); }

  inline void rotate(float angle, const QVector3D &vector) {
    top().rotate(angle, vector);
  }

  inline void rotateX(float angle) {
    top().rotate(angle, QVector3D(1., 0., 0.));
  }

  inline void rotateY(float angle) {
    top().rotate(angle, QVector3D(0., 1., 0.));
  }

  inline void rotateZ(float angle) {
    top().rotate(angle, QVector3D(0., 0., 1.));
  }

private:
  QList<QMatrix4x4> m_stack;
};

} // namespace
