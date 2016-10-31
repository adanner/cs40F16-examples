#version 410

uniform vec4 vColor;

in vec4 vPosition;
in vec2 vTexture;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 modelView;

out vec4 color;
out vec2 texCoord;

void main() 
{

    color = vColor;
    texCoord = vTexture;

    gl_Position = projection*modelView*vPosition;
} 
