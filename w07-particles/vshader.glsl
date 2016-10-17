#version 410

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 modelView;

uniform vec4 lightPos;

in vec4 vPosition;
in vec2 vTexture;
in vec3 vNormal;

out vec3 N;
out vec3 L;
out vec3 E;

out vec2 texCoord;

void main() {
  N = normalMatrix * vNormal;
  L = (camera * lightPos).xyz - (modelView * vPosition).xyz;
  E = -(modelView * vPosition).xyz; // from pt to viewer

  // texCoord = vTexture;

  gl_Position = projection * modelView * vPosition;
}
