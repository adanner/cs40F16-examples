#ifndef TIMER_H
#define TIMER_H

#include "cuda.h"
#include <sys/time.h>

class TimerBase {
public:
  virtual ~TimerBase() { /*do nothing*/
  }
  virtual void start() = 0; // Start a timer
  virtual void stop() = 0;  // Stop timer
  // Print time in seconds between last start and stop
  virtual void print() = 0;
  // Get elapsed time from last start until now in seconds
  virtual float elapsed() = 0;
};

class CPUTimer : public TimerBase {
private:
  // store start/stop times
  struct timeval startTime, stopTime, stopDiff, curDiff;
  // subtract before time from after and store in result
  void timeSub(struct timeval &result, struct timeval before,
               struct timeval after);

public:
  CPUTimer();   // Create a timer
  ~CPUTimer() { /*do nothing*/
  }
  void start();
  void stop();
  void print();
  float elapsed();
};

class GPUTimer : public TimerBase {
private:
  cudaEvent_t startTime, stopTime;

public:
  GPUTimer();
  ~GPUTimer();
  void start();
  void stop();
  void print();
  float elapsed();
};

#endif
