#version 410

uniform vec4 vColor;
uniform mat4 model; /* move object in world */
uniform mat4 view;  /* transform to eye coordinates */
uniform mat4 project; /* project to clip volume */

in vec4 vPosition;
in vec2 vTexture;

out vec4 color;
out vec2 texCoord;

void main()
{
    gl_Position = project*view*model*vPosition;
    color = vColor;
    texCoord = vTexture;
}
