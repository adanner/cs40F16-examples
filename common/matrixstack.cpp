#include "matrixstack.h"

using namespace cs40;

MatrixStack::MatrixStack() { m_stack.append(QMatrix4x4()); }

QMatrix4x4 &MatrixStack::top() {
  if (m_stack.size() == 0) {
    m_stack.append(QMatrix4x4());
  }
  return m_stack.back();
}

void MatrixStack::push() { m_stack.append(m_stack.back()); }

void MatrixStack::pop() {
  if (m_stack.size() > 0) {
    m_stack.pop_back();
  }
}
