#version 410

layout (points) in;

layout (line_strip, max_vertices=2) out;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 modelView;
uniform float nlength=0.1;

in vec3 normal[];
out vec4 color;

void main() {
	mat4 mvp = projection*camera*model;
	color = vec4(1,1,1,1); 

	//color = vec4(0.5*(normal[0]+vec3(1,1,1)),1);

	vec4 v0 = gl_in[0].gl_Position;
  gl_Position = mvp*v0;
	EmitVertex();
	gl_Position = mvp*(v0+nlength*vec4(normal[0],0));
	EmitVertex();
	EndPrimitive();
}
