#ifndef MEASURE_H
#define MEASURE_H
#pragma once

#include <chrono>

#include <string>

#include <vector>
#include<bits/stdc++.h> 

using namespace std;
using namespace std::chrono;




class Prog_Durations
{
public:
  Prog_Durations(int16_t);
  Prog_Durations(void);
  high_resolution_clock::time_point Time_Stamp, Time_Stamp_Current;
  void Start_Delay_Timer(void);
  double End_Delay_Timer(void);
  double time_delay;
  double time_delay_accum_temp;
  double time_delay_accum;
  double time_delay_max;
  double time_delay_min;
  double time_delay_avg;
  long loop_Count;
  long accum_count;
  uint16_t update_length;

  bool Trigger;
};

void Delay_Msec(float msec);

#endif /* MEASURE_H */
