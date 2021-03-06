#version 410

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform vec4 vColor;  // ambient and diffuse color
uniform vec4 vSColor; // specular color

in vec3 L;
in vec3 E;
in vec2 texCoord;

out vec4 fragColor;

void main() {
  vec4 baseColor;
  baseColor = texture(Tex0, texCoord);
  vec3 norm = texture(Tex1, texCoord).xyz * 2 - 1;
  // baseColor = vColor;
  vec4 ambient, diffuse, specular;

  vec3 NN = normalize(norm);
  vec3 EE = E;
  vec3 LL = L;
  vec3 H = normalize(LL + EE);

  float Kd = max(dot(LL, NN), 0.0);
  float Ks = pow(max(dot(NN, H), 0.0), 80);

  ambient = baseColor * 0.4;
  diffuse = baseColor * Kd * 0.5;
  specular = vec4(0, 0, 0, 1);
  if (dot(LL, NN) > 0) {
    specular = vSColor * Ks * 0.3;
  }

  fragColor = vec4((ambient + diffuse + specular).xyz, 1);
}
