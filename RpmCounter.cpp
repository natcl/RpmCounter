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

#include <RpmCounter.h>

RpmCounter::RpmCounter()
{}

void RpmCounter::begin(uint8_t pin, uint16_t nbMagnets, uint16_t interval, uint16_t mean_window, uint16_t interpol_interval) 
{
    this->pin = pin;
    this->nbMagnets = nbMagnets;
    this->interval = interval;
    this->mean_window = mean_window;
    this->interpol_interval = interpol_interval;

    nbRotations = 0;
    mean_counter = 0;
    delta = 0;
    continuous_val = 0;

    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(pin, countCallback, FALLING);
}

void RpmCounter::update()
{  
  if (rpm_cycle_timer > interval)
  {
    rpm_cycle_timer = 0;
    mean_sum = 0;

    if (mean_counter == mean_window - 1)
      mean_counter = 0;
    mean[mean_counter] = nbRotations;
    mean_counter++;

    // Nombe de tours par minute (instantané)
    instant_speed = nbRotations  * 60000 / interval / nbMagnets;

    //Serial.print("Vitesse instantanee: ");
    //Serial.println(instant_speed);
    
    // Calcul de la vitesse mean
    for (int i = 0; i < mean_window ; i++)
    {
      mean_sum+= mean[i];
    }
    mean_spead = mean_sum * 60000 / interval / mean_window / nbMagnets;
    //Serial.print("Vitesse mean: ");
    //Serial.println(mean_spead);

    delta = mean_spead - continuous_val;
    nbRotations = 0;
  }

  if (interpol_timer > interpol_interval)
  {
    interpol_timer = 0;
    continuous_val = continuous_val + delta / (interval / interpol_interval);
    if (continuous_val < 0)
      continuous_val = 0;    
    if (rpmCallback) (*rpmCallback)(continuous_val);
  }  
}


void RpmCounter::count() 
{
  nbRotations++;
}