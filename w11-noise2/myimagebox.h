#ifndef MYIMAGEBOX_H
#define MYIMAGEBOX_H

#include <QTimer>
#include <QVector3D>
#include <QtWidgets>

typedef QVector3D vec3;

class MyImageBox : public QWidget {
  Q_OBJECT
public:
  explicit MyImageBox(QWidget *parent = 0);

signals:

public slots:

  void step();

private:
  QLabel *m_imageLabel;      /* box for image */
  QScrollArea *m_scrollArea; /* scrollbars if image is too small */
  QGridLayout *m_layout;     /* declare layout of widget */

  int m_nframes;
  int m_current; /* current image on display */
  QList<QImage *> m_images;
  QTimer *m_timer;

  /* Feel free to add extra member variables and methods here */

  QImage *makeImage(float t);

  float cloud(float x, float y, float z);
  vec3 cloudColor(float x, float y, float z);
  vec3 lerp(vec3 a, vec3 b, float t);
  QRgb convertVec(vec3 clr);
};

#endif // MYIMAGEBOX_H
