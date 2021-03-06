#version 410

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 modelView;

uniform vec4 lightPos;
uniform float global_time;
uniform vec3 gravity = vec3(0, -1.2, 0);
uniform float lifetime = 2.0;

in vec4 pVelocity;
in float startTime;

out vec3 N;
out vec3 L;
out vec3 E;

out float transparency;

void main() {
  vec3 vNormal = vec3(0, 0, 1);
  N = normalMatrix * vNormal;

  vec3 vel = pVelocity.xyz;
  vec3 pos = vec3(0, 0, 0);
  float t;
  if (global_time > startTime) {
    t = mod(global_time - startTime, lifetime);
    pos = vel * t + 0.5 * gravity * t * t;
    transparency = 1.0 - t / (lifetime * lifetime);
  }
  L = (camera * lightPos).xyz - (modelView * vec4(pos,1)).xyz;
  E = -(modelView * vec4(pos,1)).xyz; // from pt to viewer
  gl_Position = projection * modelView * vec4(pos, 1);
}
