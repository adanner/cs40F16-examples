#pragma once 

#include <QtWidgets>
#include <QtOpenGL>
#include <QMatrix4x4>
#include "sphere.h"
#include "square.h"
#include "matrixstack.h"

typedef QVector4D point4;
typedef QVector4D color4;
typedef QVector3D vec3;

class MyPanelOpenGL : public QOpenGLWidget
{
    Q_OBJECT

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void keyPressEvent(QKeyEvent* event);

public:
    explicit MyPanelOpenGL(QWidget *parent = 0);
    virtual ~MyPanelOpenGL();

private:

    /* simple test shapes */
    cs40::Sphere* m_sphere;
    cs40::Square* m_square;

    int m_polymode;
    bool m_cull;
    bool m_drawSphere;
 
    cs40::MatrixStack m_matStack;

    QMatrix4x4 m_model;

		QOpenGLTexture* m_texture;

    vec3 m_angles; /* Euler angles for rotation */

    /* Shaders and program */
    QOpenGLShader *m_vertexShader;
    QOpenGLShader *m_fragmentShader;
    QOpenGLShaderProgram *m_shaderProgram;

    /* update Euler angle at index idx by amt
     * idx=0,1,2 -> x,y,z */
    void updateAngles(int idx, qreal amt);

    /* wrap a angle value to the range 0..360*/
    qreal wrap(qreal amt);

    /* update Polygon draw mode based
     * 0 : point
     * 1 : line
     * 2 : polygon */
    void updatePolyMode(int val);

    void createVBO();
    void destroyVBO();
    void createShaders();
    void destroyShaders();

signals:
    
public slots:

};
