#include <cmath>
#include <cstdlib>
#include <string>

#include <QVector3D>
#include <QColor>
#include <QImage>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

typedef QVector3D vec3;
using namespace std;

typedef enum {
  SURF0 = 0,
  SURF1,
  SURF2,
  SURF3,
  SURF4,
  SURF5,
  SURF6,
  SURF7,
  SURF8,
  SURF9,
} surf_type;

/* blend between a and b using t between 0 and 1
 * at t = 0, return a, at t = 1 return b */
vec3 lerp(vec3 a, vec3 b, float t) { return a * (1 - t) + t * b; }

/* clamp if x < 0 or x > 1 */
float clamp(float x) {
  if (x > 1) {
    return 1;
  }
  if (x < 0) {
    return 0;
  }
  return x;
}

/* shift -1->1 to 0->1 */
float adjust(float x) { return clamp((x + 1) / 2.f); }
/* convert 0..1 vec3 to 0..255 QRgb*/
QRgb convertVec(vec3 clr) {
  int r, g, b;
  r = (int)(255 * clr.x());
  g = (int)(255 * clr.y());
  b = (int)(255 * clr.z());
  return QColor(r, g, b).rgb();
}

/* convert from -1..1 noise space to 0-255 greyscale */
QRgb convertColor(float x) {
  int r, g, b;
  x = adjust(x);
  r = (int)(255 * x);
  g = (int)(255 * x);
  b = (int)(255 * x);
  return QColor(r, g, b).rgb();
}

/*Generate random float in range 0 to 1*/
float randFloat() {
  const static int digits = 10000;
  return (rand() % digits) / (1. * digits);
}

float basicFreq(float x, float y, float freq, bool periodic = true) {
  // x,y in range [0..1]
  glm::vec2 p(x * freq, y * freq);
  float val = 0.0f;
  if (periodic) {
    val = glm::perlin(p, glm::vec2(freq));
  } else {
    val = glm::perlin(p);
  }

  return val;
}

/* lumpy */
float lumpy(float x, float y) {
  float freq = 4;
  float amp = 0.5;
  float steps = 4;
  float val = 0;
  while (steps > 0) {
    val += amp * basicFreq(x, y, freq);
    amp *= 0.5;
    freq *= 2;
    steps--;
  }
  return val;
}

/* "turbulence" fabs adds discontinuity around 0. */
float turbulence(float x, float y) {
  float freq = 8;
  float amp = 0.5;
  float steps = 3;
  float val = 0;
  while (steps > 0) {
    val += fabs(amp * basicFreq(x, y, freq));
    amp *= 0.5;
    freq *= 2;
    steps--;
  }
  return val;
}

/* Use noise to modulate phase */
float marble(float x, float y) {
  return sin(3 * M_PI * (x + 0.03 * lumpy(x, y)));
}

/* Gradient with jumps */
float jumps(float x, float y) {
  float val = 5 * basicFreq(x, y, 8);
  return val - static_cast<int>(val);
}
float getVal(float x, float y, float z, surf_type surf) {

  switch (surf) {
  case SURF0:
    return basicFreq(x, y, 1);
  case SURF1:
    return basicFreq(x, y, 2);
  case SURF2:
    return basicFreq(x, y, 4);
  case SURF3:
    return basicFreq(x, y, 8);
  case SURF4:
    return lumpy(x, y);
  case SURF5:
    return turbulence(x, y);
  case SURF6:
    return marble(x, y);
  case SURF7:
    return jumps(x, y);
  default:
    return 1;
  };
}

void makeImage(string fname, float z, surf_type surf) {
  int nrows = 512, ncols = 512;

  QImage img(ncols, nrows, QImage::Format_RGB888);

  float x, y;

  for (int j = 0; j < nrows; j++) {
    for (int i = 0; i < ncols; i++) {
      x = (i * 1.0) / ncols;
      y = (j * 1.0) / nrows;
      img.setPixel(i, j, convertColor(getVal(x, y, z, surf)));
    }
  }

  QString imgName(fname.c_str());
  img.save(imgName, "PNG");
}

int main(int argc, char *argv[]) {

  makeImage("surf0.png", 0.3, SURF0);
  makeImage("surf1.png", 0.3, SURF1);
  makeImage("surf2.png", 0.3, SURF2);
  makeImage("surf3.png", 0.3, SURF3);
  makeImage("surf4.png", 0.3, SURF4);
  makeImage("surf5.png", 0.3, SURF5);
  makeImage("surf6.png", 0.3, SURF6);
  makeImage("surf7.png", 0.3, SURF7);
  return 0;
}
