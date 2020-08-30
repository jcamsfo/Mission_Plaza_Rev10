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

  uint32_t ImageWidth;
  uint32_t ImageHeight;
  uint32_t ImageChannels;
  uint32_t ImageDuration;

  // general terminology: _F is float _U is UMAt _FU is both

  Mat VideoMainAlpha;
  Mat VideoMain;

  Mat VideoChannels3[3];  
  Mat VideoChannels4[4];


  UMat VideoMain_U;
  UMat VideoMain_FU;
  UMat Color_Difference_FU3;
  UMat Y_FU1;
  UMat VideoTemp_FU3;

  string display_name;

public:
  Video_Player_With_Processing(void);
  Video_Player_With_Processing(string File_Name, string NameX);

  void setup(string File_Name, string NameX, bool Movie_Or_Still);

  void VideoSetup(string File_Name, string NameX);
  void StillSetup(string File_Name, string NameX);    

  bool Movie_Or_Still;

  void Process(void);

  void AlphaProcess(void);

  bool player_pause;

  UMat  VideoProc_FU;
  Mat   VideoDisplay;
  Mat   Alpha_Channel_F1;
  Mat   Alpha_Channel_F;  
  UMat  Alpha_Channel_FU;    
  Mat   Alpha_Channel;  

  Mat   Alpha_Channel_Inv_F;
  UMat  Alpha_Channel_Inv_FU;

  Mat  Ones_Float_Mat;
  UMat  Ones_Float_Mat_U;  





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

  double Rotating_Angle;

  bool display_on;
};

#endif /* PLAYER_CLASS_H */
