#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H

#include <QtWidgets>
#include <QtOpenGL>

class MyPanelOpenGL : public QOpenGLWidget
{
    Q_OBJECT

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public:
    explicit MyPanelOpenGL(QWidget *parent = 0);
    virtual ~MyPanelOpenGL();

private:
    void createVBOs();
    void destroyVBOs();
    void createShaders();
    void destroyShaders();


    unsigned int numVertices;
    QVector4D *vertices;
    QVector4D displacement;
    QTimer* timer;
    float phase;

    /* VAO required in core profile */
    QOpenGLVertexArrayObject *vao;
    QOpenGLBuffer *vboVertices;

    QOpenGLShader *vertexShader;
    QOpenGLShader *fragmentShader;
    QOpenGLShaderProgram *shaderProgram;

signals:

public slots:
    /* called everytime timer fires */
    void step();

};

#endif // MYPANELOPENGL_H
