#version 410

uniform sampler2D Tex0;
uniform vec4 vColor;  // ambient and diffuse color
uniform vec4 vSColor; // specular color

in vec3 N;
in vec3 L;
in vec3 E;
in float transparency;

out vec4 fragColor;

void main() {
  vec4 baseColor = vColor;

  baseColor = vColor * texture(Tex0, gl_PointCoord);
  if (baseColor.a < 0.5) {
    discard;
  }
  vec4 ambient, diffuse, specular;

  vec3 NN = normalize(N);
  vec3 EE = normalize(E);
  vec3 LL = normalize(L);
  vec3 H = normalize(LL + EE);

  float Kd = max(dot(LL, NN), 0.0);
  float Ks = pow(max(dot(NN, H), 0.0), 80);

  ambient = baseColor * 0.8;
  diffuse = baseColor * Kd * 0.5;
  specular = vec4(0, 0, 0, 1);
  if (dot(LL, NN) > 0) {
    specular = vSColor * Ks * 0.3;
  }

  // fragColor = vec4(baseColor.xyz, transparency);
  // fragColor = vec4((ambient+diffuse).xyz, transparency);
  fragColor = vec4(0, 0, 1., 1.);
}
