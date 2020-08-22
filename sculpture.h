#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H
#pragma once


#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


using namespace std;
using namespace cv;

class Video_Player_With_Processing
{
private:
  VideoCapture capMain;
  uint32_t capWidth;
  uint32_t capHeight;
  uint32_t capLength;

  Mat VideoMain;
  UMat VideoMain_U;
  UMat VideoMain_FU;
  UMat Color_Difference_FU3;
  UMat Y_FU1;
  UMat VideoTemp_FU3;

  string display_name;


public:
  Video_Player_With_Processing(string, string);
  void Process(void);

  bool player_pause;

  UMat VideoProc_FU;
  Mat VideoDisplay;

  bool Ones2x2_A;
  bool Ones3x3_A;
  bool Ones4x4_A;
  bool Ones5x5_A;
  bool Ones6x6_A;
  bool Ones7x7_A;

  float Gain;
  float Black_Level;
  float Color_Gain;
  float Image_Gamma; // different than building gamma

  bool display_on;    
};


#endif /* PLAYER_CLASS_H */
