#include "myimagebox.h"
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

MyImageBox::MyImageBox(QWidget *parent) : QWidget(parent)

{
    /* create a placeholder for an image */
    m_imageLabel = new QLabel();

    /* allow scrolling if image is too big for window */
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidget(m_imageLabel);

    /* Layout contents of widget manually (no .ui file needed) */
    m_layout = new QGridLayout();
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->addWidget(m_scrollArea);
    setLayout(m_layout);
  
		m_nframes = 1;
		for(int i=0; i< m_nframes; i++){
      m_images.append(makeImage(0.3*i));
			printf("%d\n", i);
		}
		m_current=0;
		m_imageLabel->setPixmap(QPixmap::fromImage(*m_images[m_current]));
		m_imageLabel->adjustSize();

		m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(step()));
		m_timer->start(200);
}

void MyImageBox::step(){
	m_current = (m_current+1)%m_nframes;
		m_imageLabel->setPixmap(QPixmap::fromImage(*m_images[m_current]));
		m_imageLabel->adjustSize();
}

QImage* MyImageBox::makeImage(float t) {
  int nrows = 512, ncols = 512;

  QImage* img = new QImage(ncols, nrows, QImage::Format_RGB888);

  float x, y;

  for (int j = 0; j < nrows; j++) {
    for (int i = 0; i < ncols; i++) {
      x = (i * 1.0) / ncols;
      y = (j * 1.0) / nrows;
			QRgb clr = convertVec(cloudColor(x,y,t));
      img->setPixel(i, j, clr);
    }
  }
  
	return img;
}

QRgb MyImageBox::convertVec(vec3 clr) {
  int r, g, b;
  r = (int)(255 * clr.x());
  g = (int)(255 * clr.y());
  b = (int)(255 * clr.z());
  return QColor(r, g, b).rgb();
}

vec3 MyImageBox::lerp(vec3 a, vec3 b, float t){
	return a*(1-t)+t*b;
}

vec3 MyImageBox::cloudColor(float x, float y, float t){
	vec3 white(1,1,1);
	vec3 blue(0,0,1);
  vec3 color;

  float pos = y;
	if(pos<0){ pos=0; }
	if(pos>1){ pos=1; }
	color=lerp(blue,white,3*pos*pos-2*pos*pos*pos);
  return color;
}

float MyImageBox::cloud(float x, float y, float z){
   float freq=3;
   float amp = 1;
   float steps = 5;
   float val = 0;
   while(steps > 0){
		 glm::vec3 p(x*freq, y*freq, z);
     val += fabs(amp*glm::perlin(p));
     amp*=0.6;
     freq*=2;
     steps--;
   }
   return val;
}


