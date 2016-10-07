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
in vec3 vTangent;

out vec3 L;
out vec3 E;
out vec2 texCoord;

void main() {
  vec3 n = normalize(normalMatrix * vNormal);
  vec3 t = normalize(normalMatrix * vTangent);
  vec3 b = normalize(cross(n,t));
  mat3 toTangentSpace = mat3(
      t.x, b.x, n.x,
      t.y, b.y, n.y,
      t.z, b.z, n.z);
  
  /* eye coords */
  L = (camera * lightPos).xyz - (modelView * vPosition).xyz;
  E = -(modelView * vPosition).xyz; // from pt to viewer

  /* tangent space coords */
  L = normalize(toTangentSpace*L);
  E = normalize(toTangentSpace*E);

  texCoord = vTexture;
  gl_Position = projection * modelView * vPosition;
}
