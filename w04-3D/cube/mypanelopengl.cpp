#include "mypanelopengl.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
  QOpenGLWidget(parent), m_angles(0.,0.,0.) {

    m_shaderProgram=NULL;
    m_vertexShader=NULL;
    m_fragmentShader=NULL;
    m_vboData = NULL;
    m_vao = NULL;

    m_numVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
    m_points = new point4[m_numVertices];
    m_colors = new color4[m_numVertices];

    m_vertices = new point4[8];
    m_vertex_colors = new color4[8];
  }

MyPanelOpenGL::~MyPanelOpenGL(){
  m_vboData->release();
  m_shaderProgram->release();
  destroyVBO();
  destroyShaders();
}

void MyPanelOpenGL::initializeGL()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  makeCube();
  createShaders();
  createVBO();
  setupVAO();



  glEnable(GL_DEPTH_TEST);
}

void MyPanelOpenGL::resizeGL(int w, int h)
{
  glViewport(0,0,w, h);
}

void MyPanelOpenGL::paintGL(){
  /* clear both color and depth buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  if(!m_vboData){
    return;
  }

  m_vao->bind();
  m_shaderProgram->bind();
  m_shaderProgram->setUniformValue("theta", m_angles);
  glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

  glFlush();

  m_shaderProgram->release();
  m_vao->release();

}


void MyPanelOpenGL::keyPressEvent(QKeyEvent *event)
{
  qreal step=1;
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
    default:
      QWidget::keyPressEvent(event); /* pass to base class */
  }
  update();
}

void MyPanelOpenGL::makeCube(){

  /* setup 8 corners of cube */
  m_vertices[0] = point4( -0.5, -0.5,  0.5, 1.0 );
  m_vertices[1] = point4( -0.5,  0.5,  0.5, 1.0 );
  m_vertices[2] = point4(  0.5,  0.5,  0.5, 1.0 );
  m_vertices[3] = point4(  0.5, -0.5,  0.5, 1.0 );
  m_vertices[4] = point4( -0.5, -0.5, -0.5, 1.0 );
  m_vertices[5] = point4( -0.5,  0.5, -0.5, 1.0 );
  m_vertices[6] = point4(  0.5,  0.5, -0.5, 1.0 );
  m_vertices[7] = point4(  0.5, -0.5, -0.5, 1.0 );

  /* assign one color to each corner */
  m_vertex_colors[0] = color4( 0.0, 0.0, 0.0, 1.0 );  // black
  m_vertex_colors[1] = color4( 1.0, 0.0, 0.0, 1.0 );  // red
  m_vertex_colors[2] = color4( 1.0, 1.0, 0.0, 1.0 );  // yellow
  m_vertex_colors[3] = color4( 0.0, 1.0, 0.0, 1.0 );  // green
  m_vertex_colors[4] = color4( 0.0, 0.0, 1.0, 1.0 );  // blue
  m_vertex_colors[5] = color4( 1.0, 0.0, 1.0, 1.0 );  // magenta
  m_vertex_colors[6] = color4( 1.0, 1.0, 1.0, 1.0 );  // white
  m_vertex_colors[7] = color4( 0.0, 1.0, 1.0, 1.0 );  // cyan

  /* construct faces of cube */
  quad( 1, 0, 3, 2 );
  quad( 2, 3, 7, 6 );
  quad( 3, 0, 4, 7 );
  quad( 6, 5, 1, 2 );
  quad( 4, 5, 6, 7 );
  quad( 5, 4, 0, 1 );

  delete [] m_vertices; m_vertices=NULL;
  delete [] m_vertex_colors; m_vertex_colors=NULL;
}

void MyPanelOpenGL::quad(int a, int b, int c, int d){
  /* static variable initialized once. same var (though different value)
   * used on all function calls. a 'mini global' variable */
  static int Index = 0;

  /* first triangle */
  m_colors[Index] = m_vertex_colors[a]; m_points[Index] = m_vertices[a]; Index++;
  m_colors[Index] = m_vertex_colors[a]; m_points[Index] = m_vertices[b]; Index++;
  m_colors[Index] = m_vertex_colors[a]; m_points[Index] = m_vertices[c]; Index++;
  /* second triangle */
  m_colors[Index] = m_vertex_colors[a]; m_points[Index] = m_vertices[a]; Index++;
  m_colors[Index] = m_vertex_colors[a]; m_points[Index] = m_vertices[c]; Index++;
  m_colors[Index] = m_vertex_colors[a]; m_points[Index] = m_vertices[d]; Index++;
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



void MyPanelOpenGL::createVBO(){
  destroyVBO(); //get rid of any old buffers

  m_vao = new QOpenGLVertexArrayObject(this);
  m_vao->create();

  m_vboData = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  m_vboData->create();
  m_vboData->bind();
  m_vboData->setUsagePattern(QOpenGLBuffer::StaticDraw);

  m_vboData->allocate(m_numVertices*(sizeof(point4)+sizeof(color4)));
  m_vboData->write(0,m_points,m_numVertices*sizeof(point4));
  m_vboData->write(m_numVertices*sizeof(point4),m_colors,m_numVertices*sizeof(color4));

  delete [] m_points; m_points=NULL;
  delete [] m_colors; m_colors=NULL;
}

void MyPanelOpenGL::destroyVBO(){
  if (m_vao){
    m_vao->release();
    delete m_vao; m_vao = NULL;
  }

  if (m_vboData){
    m_vboData->release();
    delete m_vboData; m_vboData=NULL;
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

void MyPanelOpenGL::setupVAO(){
  /* The VAO remembers the connections between VBOs, shader
   * variables, and buffer layout */
  if(!m_vboData or !m_shaderProgram->isLinked()){
    return;
  }

  m_vao->bind();

  m_shaderProgram->bind();
  m_vboData->bind();

  m_shaderProgram->enableAttributeArray("vPosition");
  m_shaderProgram->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4, 0);

  m_shaderProgram->enableAttributeArray("vColor");
  m_shaderProgram->setAttributeBuffer("vColor", GL_FLOAT,
      m_numVertices*sizeof(point4),4,0);


  m_shaderProgram->release();
  m_vao->release();


}

