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

  inline void Build_Clock(void);

  void Shrink_Watch(double scale_factor_h, double scale_factor_v);

  inline void Shrink_Object(UMat_Type &src, UMat_Type &src_alpha, UMat_Type &dst, UMat_Type &dst_alpha, double scale_factor_h, double scale_factor_v);

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
  uint16_t *Sampled_Buffer_RGBW, *Sampled_Buffer_RGBW_View;
  uchar *Sampled_Buffer_RGB;
  float *Sampled_Buffer_RGBW_AF;

  uint16_t *Samples_Mapped_To_Sculpture;

  // Grab the pixels from the image and store them in
  void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev5(void);

  void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev6(void);

  void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev7(void);

  void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8(void);
  

  void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8(UMat_Type & Video_In);

  void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8(UMat_Type &Vid_Src, uint16_t *RGBW_Array  );



  void Map_Subsampled_To_Sculpture(uint16_t *Vid_Sampled, uint16_t *Vid_Mapped, int *Panel_Map_Local, int xx);

  void Map_Subsampled_To_Sculpture(void);

  void Multi_Map_Image_To_Sculpture(void);

  void Add_Visible_Sample_Locations_From_Sample_Points_Map(cv::Mat src);

  void Add_Visible_Sample_Locations_From_Sample_Points_Map_Ver2(cv::Mat src);

  void Generate_Subsampled_Image_For_Test(void);

  void Display_Text_Mat(char Window_Name[100], Mat &text_window, int x, int y);

  Video_Player_With_Processing VP1x;
  Video_Player_With_Processing VP2x;

  Video_Player_With_Processing VP3x;
  Video_Player_With_Processing VP4x;

  Video_Player_With_Processing AP1x;

  bool display_on_X;

  // MAT TYPE CODING KEY:   F -> float type (vs unsigned char)     U ->  UMat_Type (vs Mat)

  // for display conversion
  UMat_Type VideoSum_FU, VideoSum_FUX, VideoSum_FUY, VideoSum_U;

  UMat_Type VideoSum_For_Output_FU;

  UMat_Type VideoSum_FUB, VideoSum_FUC, VideoSum_FUD, VideoSum_FUE, VideoSum_FUF;

  Mat VideoSum_FD;

  UMat_Type VideoSum_FUDx, VideoSum_FUDy;

  UMat_Type Alpha_Resized_FU;
  UMat_Type Alpha_Comp_FU;

  UMat_Type Small_Hand_Video;

  UMat_Type VideoSum_Resized_FU;

  UMat_Type VideoSum_Comp_FU;

  Mat VideoSum_FE;

  UMat_Type Alpha_Rotated_U;

  Mat Alpha_Rotated;

  Mat VideoSumDisplay;

  Mat Zeros_Float_Mat;

  UMat_Type Watch_With_Both_Faded_U;

  UMat_Type Alpha_Fade_Shifted_FU;

  UMat_Type Alpha_Fade_Cloned_FU;

  UMat_Type Watch_Shifted_FU, Watch_Alpha_Shifted_FU;

  UMat_Type Watch_Image;
  UMat_Type Watch_Alpha;
  UMat_Type Small_Hand_Image;
  UMat_Type Small_Hand_Alpha;
  UMat_Type Big_Hand_Image;
  UMat_Type Big_Hand_Alpha;

  UMat_Type Watch_With_Small;
  UMat_Type Watch_With_Both;

  UMat_Type Small_Hand_Image_Rotated;
  UMat_Type Small_Hand_Alpha_Rotated;

  UMat_Type Big_Hand_Image_Rotated;
  UMat_Type Big_Hand_Alpha_Rotated;

  // for mapping to sculpture
  Mat VideoSum_F;
  UMat_Type VideoSum_Small_FU;
  Mat VideoSum_Small_F;
  Mat VideoSum_Small_16;

  UMat_Type VP1x_Rotated_FU;
  UMat_Type VP3x_Rotated_FU;
  UMat_Type VP4x_Rotated_FU;

  UMat_Type Combined_Alpha_Fade_FU, Alpha_Fade_Inv_FU;

  Mat Sample_Point_Mat;

  Mat DisplayTemp;

  Mat text_window;

  Mat Subsampled_Display_Small, Subsampled_Display_Large, VideoSum_16;

  int loc_x;
  int loc_y;

  int local_oop;

  Prog_Durations time_test;

  Prog_Durations New_Timer;

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
  float    Watch_H_Location_F;

  float Watch_V_Location;
  uint64_t Fade_V_Location;

  float Watch_Angle_Inc;
  float Watch_H_Size_Inc;
  float Watch_V_Size_Inc;
  
  // int Watch_H_Location_Inc;

  float Watch_H_Location_Inc_F;

  float Watch_V_Location_Inc;

  float Watch_H_Size_Begin;
  float Watch_H_Size_End;

  bool Select_Auto, Watch_On;
  char Key_Press;
  bool Stop_Program;
  int Select_Controls;
  void KeyBoardInput(unsigned char &kp, bool &Stop_Program);

  bool Video_On;

  bool D_Clock_Selected;




bool Bypass_Output_Gain;

  float Downstream_Gain;
  float Start_Up_Gain;
  float Final_Output_Gain;

  float Sun_Gain;


  int64_t Current_Sunrise, Current_Sunset;
  bool DayTime, ColorTime;

  /// Start_Day_Sequence
  int64_t Day_Turn_On_Time;
  int64_t Enable_Day_Turn_On_Time_Trigger;
  int64_t Day_Turn_On_Time_Delayed;

  int64_t Day_Hours_Turn_On, Day_Mins_Turn_On;
  int64_t Day_Turn_On_Time_Total;

  /// Start_Night_Sequence
  int64_t Night_Turn_On_Time;
  int64_t Enable_Night_Turn_On_Time_Trigger;
  int64_t Night_Turn_On_Time_Delayed;

  int64_t Night_Hours_Turn_On, Night_Mins_Turn_On;
  int64_t Night_Turn_On_Time_Total;

  std::string Sun_Info[53][12] = {};

  int Sun_Dates_Times[12][5][3] = {}; // month  day sunrise sunset

  int data_sets, dates;
};

#endif /* VIDEO_SCULPTURE_CLASS_H */
