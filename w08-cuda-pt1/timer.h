#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

class Timer {
 private:
  //store start/stop times
  struct timeval startTime, stopTime, stopDiff, curDiff;
  //subtract before time from after and store in result
  void timeSub(struct timeval& result, 
      struct timeval before, struct timeval after);
 public:
  Timer(); //Create a timer
  void start(); //Start a timer
  void stop();  //Stop a timer
  void print(); //Print time in seconds between last start and stop
  float  elapsed(); //get elapsed time from last start until now in seconds
};

#endif
