#include "mypanelopengl.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;
using cs40::Sphere;
using cs40::Square;

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QOpenGLWidget(parent), m_angles(-125.,0.,0.) {

    m_shaderProgram=NULL;
    m_vertexShader=NULL;
    m_fragmentShader=NULL;
    m_texture = NULL;
    m_sphere = NULL;
    m_cull = false;
    m_drawSphere = true;
    m_useOrtho = true;
    m_polymode = 2;
}

MyPanelOpenGL::~MyPanelOpenGL(){
    m_shaderProgram->release();
    destroyShaders();
		if(m_texture){
			delete m_texture; m_texture=NULL;
		}
}

void MyPanelOpenGL::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CCW);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    createShaders();

    m_sphere = new Sphere(1,30,30);
    m_square = new Square(1.);
		m_texture = new QOpenGLTexture(QImage("data/earth.png").mirrored());

    m_shaderProgram->bind();
    m_view.lookAt(vec3(0,0,5),vec3(0,0,-10),vec3(0,1,0));
}

void MyPanelOpenGL::resizeGL(int w, int h)
{
    glViewport(0,0,w, h);
}

void MyPanelOpenGL::paintGL(){
    /* clear both color and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!m_shaderProgram){return;}

		m_texture->bind();
    m_shaderProgram->setUniformValue("Tex0",0);
    m_shaderProgram->setUniformValue("view",m_view);


    m_projection.setToIdentity();
    if(m_useOrtho){
      m_projection.ortho(-8,8,-8,8,1,15);
    }
    else{
      m_projection.perspective(65,1,1,15);
    }
    m_shaderProgram->setUniformValue("project",m_projection);

    updatePolyMode(m_polymode);
    if(m_cull){ glEnable(GL_CULL_FACE); }
    else{glDisable(GL_CULL_FACE); }

    if(m_drawSphere){
      m_matStack.push();
      m_matStack.translate(0,0,-6);
      m_matStack.push();
      m_matStack.translate(-3.0,0,2);
      drawSphere();
      m_matStack.pop();
      m_matStack.translate(+3.0,0,-2);
      drawSphere();
      m_matStack.pop();
    }
    else{
      m_matStack.push();
      m_matStack.translate(0,0,5);
      m_shaderProgram->setUniformValue("model",m_matStack.top());
      m_square->draw(m_shaderProgram);
      m_matStack.pop();
    }
    glFlush();

}

void MyPanelOpenGL::drawSphere(){
  m_matStack.push();
  m_matStack.scale(2);
  m_matStack.rotateX(-90);
  m_shaderProgram->setUniformValue("model",m_matStack.top());
  m_sphere->draw(m_shaderProgram);
  m_matStack.pop();
}

void MyPanelOpenGL::keyPressEvent(QKeyEvent *event)
{
    qreal step=3;
    /*Enable strong Focus on GL Widget to process key events*/
    switch(event->key()){
    case Qt::Key_X:
        if (event->text()=="x"){updateAngles(0,step);}
        else{updateAngles(0,-step);}
        break;
    case Qt::Key_Y:
        if (event->text()=="y"){ updateAngles(1,step);}
        else{ updateAngles(1,-step);}
        break;
    case Qt::Key_Z:
        if (event->text()=="z"){updateAngles(2,step);}
        else{updateAngles(2,-step);}
        break;
    case Qt::Key_C:
        m_cull = !m_cull;
        break;
    case Qt::Key_P:
        m_polymode = (m_polymode+1)%3;
        break;
    case Qt::Key_S:
        m_drawSphere = !m_drawSphere;
        break;
    case Qt::Key_O:
        m_useOrtho = !m_useOrtho;
        break;
    default:
        QWidget::keyPressEvent(event); /* pass to base class */
    }
    update();
}

void MyPanelOpenGL::updateAngles(int idx, qreal amt){
    if(idx == 0){
        m_angles.setX(m_angles.x()+amt);
    }else if(idx == 1){
        m_angles.setY(m_angles.y()+amt);
    }else if(idx == 2){
        m_angles.setZ(m_angles.z()+amt);
    }
}


qreal MyPanelOpenGL::wrap(qreal amt){
    if (amt > 360.){ return amt - 360.; }
    else if (amt < 0.){ return amt + 360.; }
    return amt;
}

void MyPanelOpenGL::updatePolyMode(int val){
    GLenum mode=GL_NONE;
    if(val==0){mode=GL_POINT;}
    else if(val==1){mode=GL_LINE;}
    else if(val==2){mode=GL_FILL;}

    if(mode != GL_NONE){
        glPolygonMode(GL_FRONT_AND_BACK, mode);
    }
}

void MyPanelOpenGL::createShaders(){

    destroyShaders(); //get rid of any old shaders

    m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!m_vertexShader->compileSourceFile("vshader.glsl")){
        qWarning() << m_vertexShader->log();
    }

    m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if(!m_fragmentShader->compileSourceFile("fshader.glsl")){
        qWarning() << m_fragmentShader->log();
    }

    m_shaderProgram = new QOpenGLShaderProgram();
    m_shaderProgram->addShader(m_vertexShader);
    m_shaderProgram->addShader(m_fragmentShader);

    if(!m_shaderProgram->link()){
        qWarning() << m_shaderProgram->log() << endl;
    }
}

void MyPanelOpenGL::destroyShaders(){

    delete m_vertexShader; m_vertexShader=NULL;
    delete m_fragmentShader; m_fragmentShader=NULL;

    if(m_shaderProgram){
        m_shaderProgram->release();
        delete m_shaderProgram; m_shaderProgram=NULL;
    }
}
