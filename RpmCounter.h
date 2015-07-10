/*The MIT License (MIT)

Copyright (c) 2015 Nathanaël Lécaudé

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef RPMCOUNTER_H
#define RPMCOUNTER_H

#include <Arduino.h>

class RpmCounter
{
public:
  RpmCounter();

  void begin(uint8_t pin, uint16_t nbMagnets, uint16_t interval, uint16_t mean_window, uint16_t interpol_interval);
  void update();
  void count();
  
  inline void setRpmCallback(void (*fptr)(float continuous_val)) 
  {
    rpmCallback = fptr;
  }

  inline void setCountCallback(void (*fptr)()) 
  {
    countCallback = fptr;
  }

protected:
  // Timers
  elapsedMillis rpm_cycle_timer;
  elapsedMillis interpol_timer;
  
  // Rpm
  uint8_t pin;
  uint16_t nbMagnets; //default 1
  uint16_t interval; //default 500 400
  uint16_t mean_window; //default 10 4
  uint16_t nbRotations;
  int mean[4];
  int mean_counter;
  uint16_t mean_sum;
  float instant_speed;
  float mean_spead;
  float delta;
  uint16_t interpol_interval; //default 10
  float continuous_val;  

  void (*rpmCallback)(float continuous_val);
  void (*countCallback)();

};

#endif
