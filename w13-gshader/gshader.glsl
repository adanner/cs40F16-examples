#version 410

layout (points) in;

layout (points) out;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 modelView;

uniform vec4 lightPos;

in vec4 vPosition[];
in vec2 vTexture[];
in vec3 vNormal[];

out vec4 color;
out vec3 N;
out vec3 L;
out vec3 E;

void main() {
  N = normalMatrix * vNormal[0]; 
	L = (camera * lightPos).xyz - (modelView * vPosition[0]).xyz;
  E = -(modelView * vPosition[0]).xyz; // from pt to viewer

  gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	//gl_Position += vec4(normalize(N),0);
	//EmitVertex();
	EndPrimitive();
}
