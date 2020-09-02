#ifndef VIDEO_SCULPTURE_CLASS_H
#define VIDEO_SCULPTURE_CLASS_H
#pragma once

#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "player_class.h"
#include "defines_Mission_Plaza.h"
#include "measure2.h"

using namespace std;
using namespace cv;

class Video_Sculpture
{
private:
public:
  Video_Sculpture(void);
  void Play_All(void);
  void Mixer(void);
  void Display(void);

  void Build_Watch(void);
  void Shrink_Watch(double scale_factor_h, double scale_factor_v);

  inline void Shrink_Object(UMat &src, UMat &src_alpha, UMat &dst, UMat &dst_alpha, double scale_factor_h, double scale_factor_v);

  // Read the 2 maps from the files  Sample_Points_Map and Sculpture_Map
  void Read_Maps(void);

  // Map for Sampling the image   sometimes call grab_map
  // Each row is different though they are all on a common grid
  vector<vector<int>> Sample_Points_Map_V;

  // Panel Map in Transbay
  // Map file for mapping the grabbed image to the way that the sculpture expects the data
  vector<int> Sculpture_Map;

  vector<vector<int>> Enclosure_Info;

  // these 2 used to Sample the image pixels using the the Sample_Points_Map
  uint16_t *Sampled_Buffer_RGBW;
  float *Sampled_Buffer_RGBW_AF;

  uint16_t *Samples_Mapped_To_Sculpture;

  // Grab the pixels from the image and store them in
  void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev5(void);

  void Map_Subsampled_To_Sculpture(uint16_t *Vid_Sampled, uint16_t *Vid_Mapped, int *Panel_Map_Local, int xx);

  void Map_Subsampled_To_Sculpture(void);

  void Multi_Map_Image_To_Sculpture(void);

  void Add_Visible_Sample_Locations_From_Sample_Points_Map(cv::Mat src);

  void Add_Visible_Sample_Locations_From_Sample_Points_Map_Ver2(cv::Mat src);

  void Generate_Subsampled_Image_For_Test(uint16_t *Buffer, bool RGBW_or_RGB, vector<vector<int>> X_Sample_Points, int Y_Start, int X_Increment, int X_Start, int Y_Increment);

  void Display_Text_Mat(char Window_Name[100], Mat & text_window, int x, int y );



      Video_Player_With_Processing VP1x;
  Video_Player_With_Processing VP2x;

  Video_Player_With_Processing VP3x;
  Video_Player_With_Processing VP4x;

  Video_Player_With_Processing AP1x;

  bool display_on_X;

  // MAT TYPE CODING KEY:   F -> float type (vs unsigned char)     U ->  UMat (vs Mat)

  // for display conversion
  UMat VideoSum_FU, VideoSum_FUX, VideoSum_FUY, VideoSum_U;

  UMat VideoSum_FUB, VideoSum_FUC, VideoSum_FUD, VideoSum_FUE, VideoSum_FUF;

  Mat VideoSum_FD;

  UMat VideoSum_FUDx, VideoSum_FUDy;

  UMat Alpha_Resized_FU;
  UMat Alpha_Comp_FU;

  UMat Small_Hand_Video;

  UMat VideoSum_Resized_FU;

  UMat VideoSum_Comp_FU;

  Mat VideoSum_FE;

  UMat Alpha_Rotated_U;

  Mat Alpha_Rotated;

  Mat VideoSumDisplay;

  Mat Zeros_Float_Mat;

  UMat Watch_With_Both_Faded_U;

  UMat Alpha_Fade_Shifted_FU;

  UMat Alpha_Fade_Cloned_FU;

  UMat Watch_Shifted_FU, Watch_Alpha_Shifted_FU;

  UMat Watch_Image;
  UMat Watch_Alpha;
  UMat Small_Hand_Image;
  UMat Small_Hand_Alpha;
  UMat Big_Hand_Image;
  UMat Big_Hand_Alpha;

  UMat Watch_With_Small;
  UMat Watch_With_Both;

  UMat Small_Hand_Image_Rotated;
  UMat Small_Hand_Alpha_Rotated;

  UMat Big_Hand_Image_Rotated;
  UMat Big_Hand_Alpha_Rotated;

  // for mapping to sculpture
  Mat VideoSum_F;
  UMat VideoSum_Small_FU;
  Mat VideoSum_Small_F;
  Mat VideoSum_Small_16;

  UMat VP1x_Rotated_FU;
  UMat VP3x_Rotated_FU;
  UMat VP4x_Rotated_FU;

  UMat Alpha_Fade_FU, Alpha_Fade_Inv_FU;

  Mat Sample_Point_Mat;

  Mat DisplayTemp;

  Mat text_window;

  int loc_x;
  int loc_y;

  int local_oop;

  Prog_Durations time_test;

  void Load_Time(void);
  time_t time_time;
  struct tm *time_X;

  int64_t Current_Hour;
  int64_t Current_Minute;
  int64_t Current_Second;

  float Watch_H_Size;
  float Watch_V_Size;
  float Watch_Angle;
  uint64_t Watch_H_Location;
  float Watch_V_Location;
  uint64_t Fade_V_Location;

  float Watch_Angle_Inc;
  float Watch_H_Size_Inc;
  float Watch_V_Size_Inc;
  int   Watch_H_Location_Inc;
  float   Watch_V_Location_Inc; 
  
   

  char  Key_Press;
  bool  Stop_Program;
  int Select_Controls;
  void KeyBoardInput(unsigned char & kp, bool & Stop_Program);
};

#endif /* VIDEO_SCULPTURE_CLASS_H */
