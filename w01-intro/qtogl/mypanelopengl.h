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

    QOpenGLBuffer *vboVertices;

    QOpenGLShader *vertexShader;
    QOpenGLShader *fragmentShader;
    QOpenGLShaderProgram *shaderProgram;

signals:
    
public slots:

};

#endif // MYPANELOPENGL_H
