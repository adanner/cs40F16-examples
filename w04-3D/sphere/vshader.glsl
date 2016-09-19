#version 410

uniform vec4 vColor;
uniform vec3 theta;
uniform mat4 model; 

in vec4 vPosition;
in vec2 vTexture;

out vec4 color;
out vec2 texCoord;

void main() 
{
    // Compute the sines and cosines of theta for each of
    //   the three axes in one computation.
    vec3 angles = radians( theta );
    vec3 c = cos( angles );
    vec3 s = sin( angles );

    // GLSL matrices are column-major
    mat4 rx = mat4( 1.0,  0.0,  0.0, 0.0,
                    0.0,  c.x,  s.x, 0.0,
                    0.0, -s.x,  c.x, 0.0,
                    0.0,  0.0,  0.0, 1.0 );

    mat4 ry = mat4( c.y, 0.0, -s.y, 0.0,
                    0.0, 1.0,  0.0, 0.0,
                    s.y, 0.0,  c.y, 0.0,
                    0.0, 0.0,  0.0, 1.0 );

    mat4 rz = mat4( c.z,  s.z, 0.0, 0.0,
                   -s.z,  c.z, 0.0, 0.0,
                    0.0,  0.0, 1.0, 0.0,
                    0.0,  0.0, 0.0, 1.0 );

    gl_Position = model*rz*ry*rx*vPosition;
    color = vColor;
    texCoord = vTexture;

} 
