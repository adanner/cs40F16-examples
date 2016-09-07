#version 410

in vec4 vPosition;
uniform vec4 displacement;

void
main()
{   
    gl_Position =  vPosition + displacement;
}
