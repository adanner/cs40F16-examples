#pragma once

#include <QtOpenGL>


namespace cs40{

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

class Sphere {
public:
    /* construct a sphere centered at origin with given radius 
		 * by dividing it into vertical slices and horizontal stacks */
    Sphere(float radius, int slices, int stacks);

    ~Sphere();

    /* draw the sphere using provided shader program  */
    void draw(QOpenGLShaderProgram* prog);

    /* Set color of Sphere */
    inline void setColor(const vec3& color){ m_color=color; m_color.setW(1.); }

    /* Get current color of object */
    inline vec4 getColor() const { return m_color; }

private:

    /* Generate Vertex Buffer Objects, but do not bind/allocate.
     * Return true if generate was a success, false otherwise
     */
    bool initVBO();


protected:
    vec4 m_color;
    QOpenGLBuffer *m_vbo;
    float m_radius;
    int m_slices;
    int m_stacks;
    int m_stripsize; //num vertices per horizontal strip;
};

} //namespace

