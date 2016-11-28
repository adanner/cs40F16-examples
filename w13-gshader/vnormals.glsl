#version 410

in vec4 vPosition;
in vec3 vNormal;

out vec3 normal;

void main() {
  normal  = vNormal;
  gl_Position = vPosition;
}
