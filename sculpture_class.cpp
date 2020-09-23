

#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <thread>
#include <iomanip>

#include "defines_Mission_Plaza.h"
#include "sculpture_class.h"
#include "player_class.h"
#include "measure2.h"
#include "process_other.h"
#include "file_io_2.h"
#include "misc.h"

using namespace std::chrono;
using namespace std;
using namespace cv;

// #define Small_Hand_Image VP3x.VideoProc_FU

typedef Vec<uint8_t, 3> Pixel_Type_8b;

typedef Vec<uint16_t, 3> Pixel_Type;

typedef Vec<uint16_t, 3> Pixel_Type_16;

typedef Vec<float, 3> Pixel_Type_F;

void alphaBlend(Mat &foreground, Mat &background, Mat &alpha, Mat &outImage)
{
  // Find number of pixels.
  int numberOfPixels = foreground.rows * foreground.cols * foreground.channels();

  // Get floating point pointers to the data matrices
  float *fptr = reinterpret_cast<float *>(foreground.data);
  float *bptr = reinterpret_cast<float *>(background.data);
  float *aptr = reinterpret_cast<float *>(alpha.data);
  float *outImagePtr = reinterpret_cast<float *>(outImage.data);

  // Loop over all pixesl ONCE
  for (
      int i = 0;
      i < numberOfPixels;
      i++, outImagePtr++, fptr++, aptr++, bptr++)
  {
    *outImagePtr = (*fptr) * (*aptr) + (*bptr) * (1 - *aptr);
  }
}

inline void rotate2(UMat_Type &src, UMat_Type &dst, double angle) //rotate function returning mat object with parametres imagefile and angle
{
  // static int Rotating_Angle;

  // Rotating_Angle++;
  // if (Rotating_Angle >= 360)
  //   Rotating_Angle = 0;
  Point2f pt(src.cols / 2., src.rows / 2.);          //point from where to rotate
  Mat r = getRotationMatrix2D(pt, angle, 1.0);       //Mat object for storing after rotation
  warpAffine(src, dst, r, Size(src.cols, src.rows)); ///applie an affine transforation to image.
}

inline void Overlay(UMat_Type &Frgnd, UMat_Type &Bkgnd, UMat_Type &Alpha, UMat_Type &Result)
{
  static UMat_Type Temp;
  multiply(Bkgnd, Alpha, Temp);
  addWeighted(Temp, 1, Frgnd, 1, 0, Result);
}

// Define a pixel
typedef Point3_<uint8_t> Pixel;
typedef Point3_<float> Pixel3F;

void BW_Convert(Pixel3F &pixel)
{
  pixel.x = 256 * ((pixel.x >= 255) ? 255 : (pixel.x <= 0) ? 0 : pixel.x);
  pixel.y = 256 * ((pixel.y >= 255) ? 255 : (pixel.y <= 0) ? 0 : pixel.y);
  pixel.z = 256 * ((pixel.z >= 255) ? 255 : (pixel.z <= 0) ? 0 : pixel.z);
}

struct Operator
{
  void operator()(Pixel3F &pixel, const int *position) const
  {
    BW_Convert(pixel);
  }
};

Video_Sculpture::Video_Sculpture(void)
{

  Sampled_Buffer_RGBW = new uint16_t[Sculpture_Size_RGBW]; //  16 bit   4 * 13116  52464

  Sampled_Buffer_RGBW = new uint16_t[Sculpture_Size_RGBW]; //  16 bit   4 * 13116  52464

  Sampled_Buffer_RGBW_View = new uint16_t[Sculpture_Size_RGBW]; //  16 bit   4 * 13116  52464

  Sampled_Buffer_RGBW_AF = new float[Sculpture_Size_RGBW]; //  16 bit   4 * 13116  52464
  Samples_Mapped_To_Sculpture = new uint16_t[Buffer_W_Gaps_Size_RGBW_Extra];

  Sampled_Buffer_RGB = new uchar[Sculpture_Size_RGB]; //  16 bit   4 * 13116  52464

  // pictures for the tower
  // VP1x.setup("../../Movies/TSB3X.mov", "0");
  // VP2x.setup("../../Movies/flagblack.mov", "1");

  // VP1x.setup("../../Movies/rainbow.mp4", "0", 1);
  // VP2x.VideoSetup("../../Movies/comp4_264.mov", "1");
  // VP3x.StillSetup("../../Movies/alpha_trans.tif", "3");

// VideoSetup(string File_Name, string NameX, float Gain_In, float Black_Level_In, float Color_Gain_In, float Gamma_In)
  VP1x.VideoSetup("../../Movies/wrxz100.mov", "0", 1, -.5, 1, .5);

  // VP1x.VideoSetup("../../Movies/wrxz100.mov", "0");
  //VP2x.StillSetupRev2("../../Movies/watch_SM.tif", "1");

  // StillSetupWithAlpha(string File_Name, string NameX, float Gain_In, float Black_Level_In, float Color_Gain_In, float Gamma_In)
  VP2x.StillSetupWithAlpha("../../Movies/wtc3s.tif", "1", 1, -.5, 1, .5);

  VP3x.StillSetupWithAlpha("../../Movies/smallhand.tif", "2");
  VP4x.StillSetupWithAlpha("../../Movies/bighand.tif", "3");

  AP1x.AlphaSetupNoProcess("../../Movies/fader_soft_3C.tif", "4");

  // CODING KEY:   F -> float type (vs unsigned char)     U ->  UMat_Type (vs Mat)
  // CREATES NOT NEEDED
  // VP1x_Rotated_FU.create(IMAGE_COLS, IMAGE_ROWS, CV_32FC3);
  // VP3x_Rotated_FU.create(IMAGE_COLS, IMAGE_ROWS, CV_32FC3);
  // // VP4x_Rotated_FU.create(IMAGE_COLS, IMAGE_ROWS, CV_32FC3);
  // VideoSum_FU.create(IMAGE_COLS, IMAGE_ROWS, CV_32FC3);
  // VideoSum_FUX.create(IMAGE_COLS, IMAGE_ROWS, CV_32FC3);
  // VideoSum_FUY.create(IMAGE_COLS, IMAGE_ROWS, CV_32FC3);
  // VideoSum_U.create(IMAGE_COLS, IMAGE_ROWS, CV_8UC3);
  // VideoSumDisplay.create(IMAGE_COLS, IMAGE_ROWS, CV_8UC3);
  // VideoSum_F.create(IMAGE_COLS, IMAGE_ROWS, CV_32FC3);
  // VideoSum_Small_FU.create(IMAGE_COLS / 4, IMAGE_ROWS / 4, CV_32FC3);
  // VideoSum_Small_F.create(IMAGE_COLS / 4, IMAGE_ROWS / 4, CV_32FC3);
  // VideoSum_Small_16.create(IMAGE_COLS / 4, IMAGE_ROWS / 4, CV_16UC3);

  text_window.create(200, 600, CV_8UC3);

  Subsampled_Display_Small.create(SAMPLE_ROWS, SAMPLE_COLS, CV_8UC3);

  Key_Press = -1;

  display_on_X = false;

  Video_On = true;

  local_oop = 0;
  Watch_Angle = 360;
  Watch_Angle_Inc = 1;
  Watch_H_Size = 0.55;
  Watch_V_Size = 0.55;
  Watch_H_Size_Inc = .01;
  Watch_V_Size_Inc = .01;
  Watch_H_Location = 0;
  Watch_H_Location_F = 0;

  Fade_V_Location = 50;

  Watch_V_Location_Inc = .25;

  Watch_V_Location = -100;

  Watch_Loop_Timer = 0;

  Watch_H_Location_Inc_F = .5;

  Watch_H_Size_Begin = .5;
  Watch_H_Size_End = 1;

  Select_Controls = 0;
  Select_Auto = true;
  Watch_On = true;
  D_Clock_Selected = false;

  Downstream_Gain = .2;
  Start_Up_Gain = 0;

  Bypass_Output_Gain = false;
};

// void Video_Sculpture::Read_Maps(void)
// {
//   Read_2D(Sample_Points_Map_V, "../../Maps/Day_For_Night_Sample_Map.csv");

//   Read_1D(Sculpture_Map, "../../Maps/Day_For_Night_Sculpture_Map.csv");

//   // ignore the 1st line of descriptions
//   Read_2D_Ignore(Enclosure_Info, "../../Maps/Day_For_Night_Enclosure_Info.csv", 1);
// }

void Video_Sculpture::Read_Maps(void)
{
  Read_2D(Sample_Points_Map_V, "../../Maps/Mission_Plaza_Sample_Map.csv");

  Read_1D(Sculpture_Map, "../../Maps/Mission_Plaza_Sculpture_Map.csv");

  dates = Read_YAML_Data("../SF-Sunrise-Sunset.yml", Sun_Info);

  Convert_Sun_File(Sun_Info, Sun_Dates_Times);

  for (int aa = 0; aa < 12; aa++)
    for (int bb = 0; bb < 5; bb++)
      printf("%d  %d   %d  %d %d \n", aa, bb, Sun_Dates_Times[aa][bb][0], Sun_Dates_Times[aa][bb][1], Sun_Dates_Times[aa][bb][2]);
  //  exit(0);

  // ignore the 1st line of descriptions
  // Read_2D_Ignore(Enclosure_Info, "../../Maps/Day_For_Night_Enclosure_Info.csv", 1);
}

void Video_Sculpture::Load_Time(void)
{
  time(&time_time);
  time_X = localtime(&time_time);
  Current_Hour = time_X->tm_hour;
  Current_Minute = time_X->tm_min;
  Current_Second = time_X->tm_sec;

  //  cout << endl << "Current Day, Date and Time is = " << Current_Hour << " " << Current_Minute << " "  <<  Current_Second << endl ; // asctime(time_X);
}

inline void Video_Sculpture::Build_Watch(void)
{
  static int64_t Big_Hand_Angle, Small_Hand_Angle;

  // Current_Hour = 11;
  // Current_Minute = 58;

  if (Current_Hour >= 12)
    Current_Hour = Current_Hour - 12;

  Big_Hand_Angle = 360 - Current_Minute * 6;

  // 360 /12
  Small_Hand_Angle = 360 - ((Current_Hour * 30) + Current_Minute / 2); //  -  Current_Minute/2 );

  // these are just for clarifying naming only pointers not copied Mat data
  Watch_Image = VP2x.VideoProc_FU;
  // Watch_Alpha = VP2x.Alpha_Channel_FU;

  Small_Hand_Image = VP3x.VideoProc_FU;
  Small_Hand_Alpha = VP3x.Alpha_Channel_FU;
  Big_Hand_Image = VP4x.VideoProc_FU;
  Big_Hand_Alpha = VP4x.Alpha_Channel_FU;

  // small hand
  rotate2(Small_Hand_Image, Small_Hand_Image_Rotated, Small_Hand_Angle);
  rotate2(Small_Hand_Alpha, Small_Hand_Alpha_Rotated, Small_Hand_Angle);
  Overlay(Small_Hand_Image_Rotated, Watch_Image, Small_Hand_Alpha_Rotated, Watch_With_Small);

  // big hand
  rotate2(Big_Hand_Image, Big_Hand_Image_Rotated, Big_Hand_Angle);
  rotate2(Big_Hand_Alpha, Big_Hand_Alpha_Rotated, Big_Hand_Angle);
  Overlay(Big_Hand_Image_Rotated, Watch_With_Small, Big_Hand_Alpha_Rotated, Watch_With_Both);
}

inline void Video_Sculpture::Build_Clock(void)
{

  // static int64_t Big_Hand_Angle, Small_Hand_Angle;
  // these are just for clarifying naming only pointers not copied Mat data
  // Watch_Image = VP2x.VideoProc_FU;
  // Watch_Alpha = VP2x.Alpha_Channel_FU;

  // Small_Hand_Image = VP3x.VideoProc_FU;
  // Small_Hand_Alpha = VP3x.Alpha_Channel_FU;
  // Big_Hand_Image = VP4x.VideoProc_FU;
  // Big_Hand_Alpha = VP4x.Alpha_Channel_FU;

  // // small hand
  // rotate2(Small_Hand_Image, Small_Hand_Image_Rotated, Small_Hand_Angle);
  // rotate2(Small_Hand_Alpha, Small_Hand_Alpha_Rotated, Small_Hand_Angle);
  // Overlay(Small_Hand_Image_Rotated, Watch_Image, Small_Hand_Alpha_Rotated, Watch_With_Small);

  // // big hand
  // rotate2(Big_Hand_Image, Big_Hand_Image_Rotated, Big_Hand_Angle);
  // rotate2(Big_Hand_Alpha, Big_Hand_Alpha_Rotated, Big_Hand_Angle);
  // Overlay(Big_Hand_Image_Rotated, Watch_With_Small, Big_Hand_Alpha_Rotated, Watch_With_Both);

  Watch_With_Both = VP2x.VideoProc_FU;
}

inline void Video_Sculpture::Shrink_Watch(double scale_factor_h, double scale_factor_v)
{
  resize(Watch_With_Both, VideoSum_Resized_FU, Size(), scale_factor_h, scale_factor_v, INTER_LINEAR);
  loc_x = (Watch_With_Both.cols - VideoSum_Resized_FU.cols) / 2;
  loc_y = (Watch_With_Both.rows - VideoSum_Resized_FU.rows) / 2;
  VideoSum_Comp_FU = VP2x.Zeros_Float_Mat_U.clone();
  VideoSum_Resized_FU.copyTo(VideoSum_Comp_FU(cv::Rect(loc_x, loc_y, VideoSum_Resized_FU.cols, VideoSum_Resized_FU.rows)));
  resize(VP2x.Alpha_Channel_Inv_FU, Alpha_Resized_FU, Size(), scale_factor_h, scale_factor_v, INTER_LINEAR);
  Alpha_Comp_FU = VP2x.Zeros_Float_Mat_U.clone();
  Alpha_Resized_FU.copyTo(Alpha_Comp_FU(cv::Rect(loc_x, loc_y, Alpha_Resized_FU.cols, Alpha_Resized_FU.rows)));
}

// INTER_NEAREST        = 0,
// /** bilinear interpolation */
// INTER_LINEAR         = 1,
// /** bicubic interpolation */
// INTER_CUBIC          = 2,

inline void Video_Sculpture::Shrink_Object(UMat_Type &src, UMat_Type &src_alpha, UMat_Type &dst, UMat_Type &dst_alpha, double scale_factor_h, double scale_factor_v)
{
  static UMat_Type resized;
  static UMat_Type resized_alpha;
  static int x, y;
  resize(src, resized, Size(), scale_factor_h, scale_factor_v, INTER_LINEAR);
  x = (int)(.5 + (((float)(src.cols - resized.cols)) / 2));
  y = (int)(.5 + (((float)(src.rows - resized.rows)) / 2));
  dst = VP2x.Zeros_Float_Mat_U.clone();
  resized.copyTo(dst(cv::Rect(x, y, resized.cols, resized.rows)));
  resize(src_alpha, resized_alpha, Size(), scale_factor_h, scale_factor_v, INTER_LINEAR);
  dst_alpha = VP2x.Zeros_Float_Mat_U.clone();
  resized_alpha.copyTo(dst_alpha(cv::Rect(x, y, resized_alpha.cols, resized_alpha.rows)));
}

void Video_Sculpture::Display_Text_Mat(char Window_Name[100], Mat &text_window, int x, int y)
{

  int Font_Type = FONT_HERSHEY_DUPLEX;
  // text_window.setTo(Scalar(100, 100, 20));
  // Scalar Onnn = Scalar(200, 200, 200);
  // Scalar Offf = Scalar(100, 100, 120);

  text_window.setTo(Scalar(20, 20, 20));
  Scalar Onnn = Scalar(140, 140, 140);
  Scalar Offf = Scalar(60, 60, 60);

  Scalar Water_Color = (Select_Controls == 0) ? Onnn : Offf;
  Scalar Watch_Color = (Select_Controls == 1) ? Onnn : Offf;

  putText(text_window, format("Water Gain %.2f  C Gain %.2f  Black %.2f  Gamma %.2f  Video On %d", VP1x.Gain, VP1x.Color_Gain, VP1x.Black_Level, VP1x.Image_Gamma, Video_On), Point(10, 20), Font_Type, .4, Water_Color, 0, LINE_AA);

  putText(text_window, format("Watch Gain %.2f  C Gain %.2f  Black %.2f  Gamma %.2f  ", VP2x.Gain, VP2x.Color_Gain, VP2x.Black_Level, VP2x.Image_Gamma), Point(10, 50), Font_Type, .4, Watch_Color, 0, LINE_AA);
  putText(text_window, format("H Speed %.2f  V Speed %.2f size %.2f  AutoSize %d  Watch %d", Watch_H_Location_Inc_F, Watch_V_Location_Inc, Watch_H_Size, Select_Auto, Watch_On), Point(10, 70), Font_Type, .4, Watch_Color, 0, LINE_AA);

  putText(text_window, format("Downstream Gain %.2f   Sun_Gain  %.2f   Final_Output_Gain %.2f", Downstream_Gain, Sun_Gain, Final_Output_Gain), Point(10, 120), Font_Type, .4, Onnn, 0, LINE_AA);

  putText(text_window, format("Out Gain Bypassed  %d ", Bypass_Output_Gain), Point(10, 150), Font_Type, .4, Onnn, 0, LINE_AA);

  // Scalar  	color,
  // 	int  	thickness = 1,
  // 	int  	lineType = LINE_8,
  // 	bool  	bottomLeftOrigin = false
  // )

  // LINE_AA

  namedWindow(Window_Name);
  // moveWindow(Window_Name, x,y);
  imshow(Window_Name, text_window);
}

void Video_Sculpture::KeyBoardInput(unsigned char &kp, bool &Stop_Program)
{
  static unsigned char last_kp, kp_2ago;
  static string file_name;
  static int New_Watch_Number, New_Water_Number, New_Image_Number;

  if (kp != 255)
  {
    if (kp == 27)
      Stop_Program = true;
    else if ((kp == 99) & (last_kp == 227)) // "Cntrl C"
    {
      Stop_Program = true;
    }
    else if (kp == 's')
    {
      Select_Controls++;
      if (Select_Controls >= 2)
        Select_Controls = 0;
    }

    else if (kp == 'd')
    {
      display_on_X = !display_on_X;
    }

    else if (kp == 'q')
    {
      Video_On = !Video_On;
    }

    else if ((kp_2ago == 'w') & isdigit(last_kp) & isdigit(kp))
    {

      Fade_V_Location = 50;
      D_Clock_Selected = false;
      New_Watch_Number = kp - '0' + 10 * (last_kp - '0');

      if ((New_Watch_Number >= 0) && (New_Watch_Number <= 11))
      {
        file_name = "../../Movies/wtc" + to_string(New_Watch_Number) + "s.tif"; //   10s.tif" ;
        VP2x.StillSetupWithAlpha(file_name, "1");
      }

      cout << endl
           << endl
           << "  file_name  " << file_name << endl
           << endl;
    }

    else if ((kp_2ago == 't') & isdigit(last_kp) & isdigit(kp))
    {

      D_Clock_Selected = true;
      New_Watch_Number = kp - '0' + 10 * (last_kp - '0');

      Fade_V_Location = 50;

      if (New_Watch_Number <= 5)
      {
        if (New_Watch_Number == 0)
          file_name = "../../Movies/dclock1.tif"; //   10s.tif" ;
        else if (New_Watch_Number == 1)
          file_name = "../../Movies/1253.tif"; //   10s.tif" ;
        else if (New_Watch_Number == 2)
          file_name = "../../Movies/125324.tif"; //   10s.tif" ;
        else if (New_Watch_Number == 3)
          file_name = "../../Movies/1253xx.tif"; //   10s.tif" ;
        else if (New_Watch_Number == 4)
          file_name = "../../Movies/125324z.tif"; //   10s.tif" ;
        else if (New_Watch_Number == 5)
        {
          file_name = "../../Movies/hourglass.tif"; //   10s.tif"
          Fade_V_Location = 68;
        }
        VP2x.StillSetupWithAlpha(file_name, "1");
        ;
        VP2x.StillSetupWithAlpha(file_name, "1");
      }

      cout << endl
           << endl
           << "  file_name  " << file_name << endl
           << endl;
    }

    else if ((kp_2ago == 'r') & isdigit(last_kp) & isdigit(kp))
    {
      New_Water_Number = kp - '0' + 10 * (last_kp - '0');
      file_name = "../../Movies/water" + to_string(New_Water_Number) + ".mov"; //   10s.tif" ;
      if ((New_Water_Number >= 0) && (New_Water_Number <= 2))
        VP1x.VideoSetup(file_name, "0");
      cout << endl
           << endl
           << "  file_name  " << file_name << endl
           << endl;
    }

    else if ((kp_2ago == 'p') & isdigit(last_kp) & isdigit(kp))
    {
      New_Image_Number = kp - '0' + 10 * (last_kp - '0');
      file_name = "../../Movies/";
      if ((New_Image_Number >= 0) && (New_Image_Number <= 55))
      {
        if (New_Image_Number == 0)
          file_name += "black-MPZ.mp4";
        else if (New_Image_Number == 1)
          file_name += "rgbwycmw-MPZ.mp4";
        else if (New_Image_Number == 2)
          file_name += "rainbow.mp4";
        else if (New_Image_Number == 3)
          file_name += "waves-MPZ.mp4";
        else if (New_Image_Number == 4)
          file_name += "comp1_264.mov";
        else if (New_Image_Number == 5)
          file_name += "comp4_264.mov";
        else if (New_Image_Number == 4)
          file_name += "comp5_264.mov";
        else if (New_Image_Number == 5)
          file_name += "comp6_264.mov";

        else if (New_Image_Number == 6)
          file_name += "stairs.mov";

        else if (New_Image_Number == 7)
          file_name += "swim.mp4";

        else if (New_Image_Number == 8)
          file_name += "waves2.mp4";

        else if (New_Image_Number == 9)
          file_name += "run.mp4";

        else if (New_Image_Number == 10)
          file_name += "w40.mp4";

        else if (New_Image_Number == 11)
          file_name += "w60.mp4";

        else if (New_Image_Number == 12)
          file_name += "w80.mp4";

        else if (New_Image_Number == 13)
          file_name += "w80bw.mp4";

        else if (New_Image_Number == 14)
          file_name += "w100.mp4";

        else if (New_Image_Number == 15)
          file_name += "water40BW_obj50.mov";

        else if (New_Image_Number == 16)
          file_name += "water40color_obj30.mov";

        else if (New_Image_Number == 17)
          file_name += "water40color_obj100.mov";

        else if (New_Image_Number == 18)
          file_name += "water60BW_obj50.mov";

        else if (New_Image_Number == 19)
          file_name += "water60color_obj30.mov";

        else if (New_Image_Number == 20)
          file_name += "water60color_obj100.mov";

        else if (New_Image_Number == 21)
          file_name += "water80BW_obj50.mov";

        else if (New_Image_Number == 22)
          file_name += "water80color_obj30.mov";

        else if (New_Image_Number == 23)
          file_name += "water80color_obj100.mov";

        else if (New_Image_Number == 24)
          file_name += "water100BW_obj50.mov";

        else if (New_Image_Number == 25)
          file_name += "water100color_obj30.mov";

        else if (New_Image_Number == 26)
          file_name += "water100color_obj100.mov";

        else if (New_Image_Number == 27)
          file_name += "obj_in_water2.mp4";

        else if (New_Image_Number == 28)
          file_name += "victorian_in_water.mov";

        else if (New_Image_Number == 29)
          file_name += "vic_slow_rotation.mov";

        else if (New_Image_Number == 30)
          file_name += "vic_fast_rotation.mov";

        else if (New_Image_Number == 31)
          file_name += "clock_spinning.mov";

        else if (New_Image_Number == 32)
          file_name += "clk_spinning_in_wat.mov";

        else if (New_Image_Number == 33)
          file_name += "clock.mov";

        else if (New_Image_Number == 34)
          file_name += "clock_in_water.mov";

        else if (New_Image_Number == 35)
          file_name += "cwfm.mov";

        else if (New_Image_Number == 36)
          file_name += "cwfr.mov";

        else if (New_Image_Number == 37)
          file_name += "big_watch.mov";

        else if (New_Image_Number == 38)
          file_name += "wfm.mov";

        else if (New_Image_Number == 39)
          file_name += "wpm.mov";

        else if (New_Image_Number == 40)
          file_name += "fire.mp4";

        else if (New_Image_Number == 41)
          file_name += "birds1.mp4";

        else if (New_Image_Number == 42)
          file_name += "birds2.mp4";

        else if (New_Image_Number == 43)
          file_name += "street.mp4";

        else if (New_Image_Number == 44)
          file_name += "waves.mp4";

        else if (New_Image_Number == 45)
          file_name += "paint.mp4";

        else if (New_Image_Number == 46)
          file_name += "wrxz100.mov";

        else if (New_Image_Number == 47)
          file_name += "wrxz85.mov";

        else if (New_Image_Number == 48)
          file_name += "wrxz70.mov";

        else if (New_Image_Number == 49)
          file_name += "wr20z100.mov";

        else if (New_Image_Number == 50)
          file_name += "wr20z85.mov";

        else if (New_Image_Number == 51)
          file_name += "wr20z70.mov";

        else if (New_Image_Number == 52)
          file_name += "wrn20z100.mov";

        else if (New_Image_Number == 53)
          file_name += "wrn20z85.mov";

        else if (New_Image_Number == 54)
          file_name += "wrn20z70.mov";

        else if (New_Image_Number == 55)
          file_name += "wrxz100smooth.mov";

        VP1x.VideoSetup(file_name, "0");
      }

      cout << endl
           << endl
           << "  file_name  " << file_name << endl
           << endl;
    }

    // else if(  (kp_2ago == 's') & isdigit(last_kp)   & isdigit(c) )
    // {

    //     First_Sequence_Image_X = c - '0' +  10 * (last_kp - '0') ;
    //     Sequence_On = true; // start on 's'
    // }

    if (kp == '<')
    {
      Downstream_Gain -= .01;
      if (Downstream_Gain < 0)
        Downstream_Gain = 0;
    }
    else if (kp == '>')
    {
      Downstream_Gain += .01;
      if (Downstream_Gain > 2)
        Downstream_Gain = 1;
    }

    else if (kp == 'f')
    {
      Bypass_Output_Gain = !Bypass_Output_Gain;
    }

    else if (Select_Controls == 0)
    {

      if (kp == ',')
      {
        VP1x.Gain -= .05;
        if (VP1x.Gain < 0)
          VP1x.Gain = 0;
      }
      else if (kp == '.')
      {
        VP1x.Gain += .05;
        if (VP1x.Gain > 2)
          VP1x.Gain = 2;
      }

      else if (kp == 'k')
      {
        VP1x.Color_Gain -= .05;
        if (VP1x.Color_Gain < 0)
          VP1x.Color_Gain = 0;
      }
      else if (kp == 'l')
      {
        VP1x.Color_Gain += .05;
        if (VP1x.Color_Gain > 2)
          VP1x.Color_Gain = 2;
      }

      else if (kp == ';')
      {
        VP1x.Image_Gamma -= .05;
        if (VP1x.Image_Gamma < 0)
          VP1x.Image_Gamma = 0;
      }
      else if (kp == 39)
      {
        VP1x.Image_Gamma += .05;
        if (VP1x.Image_Gamma > 1)
          VP1x.Image_Gamma = 1;
      }

      else if (kp == '[')
      {
        VP1x.Black_Level -= .05;
        if (VP1x.Black_Level < -1)
          VP1x.Black_Level = -1;
      }
      else if (kp == ']')
      {
        VP1x.Black_Level += .05;
        if (VP1x.Black_Level > 1)
          VP1x.Black_Level = 1;
      }
    }

    else if (Select_Controls == 1)
    {
      if (kp == ',')
      {
        VP2x.Gain -= .05;
        if (VP2x.Gain < 0)
          VP2x.Gain = 0;
        VP2x.Process();
      }
      else if (kp == '.')
      {
        VP2x.Gain += .05;
        if (VP2x.Gain > 2)
          VP2x.Gain = 2;
        VP2x.Process();
      }

      else if (kp == 'k')
      {
        VP2x.Color_Gain -= .05;
        if (VP2x.Color_Gain < 0)
          VP2x.Color_Gain = 0;
        VP2x.Process();
      }
      else if (kp == 'l')
      {
        VP2x.Color_Gain += .05;
        if (VP2x.Color_Gain > 2)
          VP2x.Color_Gain = 2;
        VP2x.Process();
      }

      else if (kp == ';')
      {
        VP2x.Image_Gamma -= .05;
        if (VP2x.Image_Gamma < 0)
          VP2x.Image_Gamma = 0;
        VP2x.Process();
      }
      else if (kp == 39)
      {
        VP2x.Image_Gamma += .05;
        if (VP2x.Image_Gamma > 1)
          VP2x.Image_Gamma = 1;
        VP2x.Process();
      }

      else if (kp == '[')
      {
        VP2x.Black_Level -= .05;
        if (VP2x.Black_Level < -1)
          VP2x.Black_Level = -1;
        VP2x.Process();
      }
      else if (kp == ']')
      {
        VP2x.Black_Level += .05;
        if (VP2x.Black_Level > 1)
          VP2x.Black_Level = 1;
        VP2x.Process();
      }

      if (kp == 'g')
      {
        Watch_H_Location_Inc_F -= .1;
        if (Watch_H_Location_Inc_F < 0)
          Watch_H_Location_Inc_F = 0;
      }
      else if (kp == 'h')
      {
        Watch_H_Location_Inc_F += .1;
        if (Watch_H_Location_Inc_F > 5.)
          Watch_H_Location_Inc_F = 5.;
      }

      if ((kp == 'c') && (Watch_V_Location_Inc >= .05))
      {

        Watch_V_Location_Inc -= .05;
      }
      else if ((kp == 'v') && (Watch_V_Location_Inc <= 2))
      {
        Watch_V_Location_Inc += .05;
      }

      if ((kp == 'u') && (Watch_H_Size >= .3))
      {
        Watch_H_Size -= .05;
      }
      else if ((kp == 'i') && (Watch_H_Size <= .9))
      {
        Watch_H_Size += .05;
      }

      else if (kp == 'y')
      {
        Select_Auto = !Select_Auto;
      }

      else if (kp == 'z')
      {
        Watch_On = !Watch_On;
      }
    }

    cout << " key  " << (uint)kp << " last key  " << (uint)last_kp << endl;

    kp_2ago = last_kp;
    last_kp = kp;
  }
}

void Video_Sculpture::Play_All(void)
{

  Sun_Gain = Day_Night_Final_Gain(Sun_Dates_Times, Current_Sunrise, Current_Sunset);

  // all of the other images are stills!!!!!!!!!!!!!!!!!!
  VP1x.Process();

  // std::thread t1(&Video_Player_With_Processing::Process, &VP1x);
  // std::thread t2(&Video_Player_With_Processing::Process, &VP2x);
  // std::thread t3(&Video_Player_With_Processing::Process, &VP3x);
  // std::thread t4(&Video_Player_With_Processing::Process, &VP4x);
  // std::thread t5(&Video_Player_With_Processing::AlphaProcess, &VP2x);
  // std::thread t6(&Video_Player_With_Processing::AlphaProcess, &VP3x);
  // std::thread t7(&Video_Player_With_Processing::AlphaProcess, &VP4x);
  // t1.join();
  // t2.join();
  // t3.join();
  // t4.join();
  // t5.join();
  // t6.join();
  // t7.join();

  // std::thread t1(&Video_Player_With_Processing::Process, &VP1x);
  // std::thread t2(&Video_Player_With_Processing::Process, &VP2x);
  // std::thread t3(&Video_Player_With_Processing::Process, &VP3x);
  // std::thread t4(&Video_Player_With_Processing::Process, &VP4x);

  // t1.join();
  // t2.join();
  // t3.join();
  // t4.join();

  // std::thread t5(&Video_Player_With_Processing::AlphaProcess, &VP2x);
  // std::thread t6(&Video_Player_With_Processing::AlphaProcess, &VP3x);
  // std::thread t7(&Video_Player_With_Processing::AlphaProcess, &VP4x);
  // t5.join();
  // t6.join();
  // t7.join();

  // no difference with threading  seems automatic
  // VP1x.Process();
  // VP2x.Process();
}

// void Video_Sculpture::Mixer(void)
// {

//   Rotating_Angle++;
//   if (Rotating_Angle >= 360)
//     Rotating_Angle = 0;
//   rotate2(VP1x.VideoProc_FU, VP1x_Rotated_FU, Rotating_Angle);

//   addWeighted(VP1x_Rotated_FU, .5, VP2x.VideoProc_FU, .5, 0, VideoSum_FUX);

//   multiply(VideoSum_FUX, VP3x.Alpha_Channel_FU, VideoSum_FUY);

//   addWeighted(VideoSum_FUY, 1, VP3x.VideoProc_FU, 1, 0, VideoSum_FU);
// }

// VP1x.setup("../../Movies/waterloop250.mov", "0", 1);
// VP2x.setup("../../Movies/watch_SM.tif", "1", 0);
// VP3x.setup("../../Movies/smallhand.tif", "2", 0);
// VP4x.setup("../../Movies/bighand.tif", "3", 0);

void Video_Sculpture::Mixer(void)
{

  // uint64_t Watch_Size;
  // uint64_t Watch_Size_Inc;
  // uint64_t Watch_Angle;
  // uint64_t Watch_Angle_Inc;

  // uint64_t Watch_H_Location;
  // uint64_t Watch_V_Location;
  // uint64_t Fade_V_Location;

  // Rotating_Angle++;
  // if (Rotating_Angle >= 360)
  //   Rotating_Angle = 0;

  Watch_V_Location += Watch_V_Location_Inc;
  if (Watch_V_Location >= 200)
  {
    Watch_Loop_Timer++;
    Watch_V_Location = 200;
  }

   if(Watch_Loop_Timer >= 200)  // in frames  210 = 7 seconds
   {
     Watch_V_Location = -200;
     Watch_Loop_Timer = 0;
   }
  // if (Watch_V_Location >= 200)
  //   Watch_V_Location = -200;

  // Watch_Loop_Timer

  // Watch_V_Location = 0;

  // if (Watch_H_Size >= .8)
  //   Watch_H_Size_Inc = -.0001;
  // else if (Watch_H_Size <= .3)
  //   Watch_H_Size_Inc = .001;
  // Watch_H_Size += Watch_H_Size_Inc;

  // Watch_H_Size = .55;

  // if (Watch_V_Size >= .8)
  //   Watch_V_Size_Inc = -.0001;
  // else if (Watch_V_Size <= .3)
  //   Watch_V_Size_Inc = .001;
  // Watch_V_Size += Watch_V_Size_Inc;

  float V_Percent = (Watch_V_Location + 200) / 400;

  float Watch_H_Size_Auto = V_Percent * (Watch_H_Size_End - Watch_H_Size_Begin) + Watch_H_Size_Begin;

  if (Select_Auto)
    Watch_H_Size = Watch_H_Size_Auto;

  Watch_V_Size = Watch_H_Size;

  if (Watch_Angle >= 370)
    Watch_Angle_Inc = -.2;
  else if (Watch_Angle <= 350)
    Watch_Angle_Inc = .2;
  Watch_Angle += Watch_Angle_Inc;

  // Watch_Angle = 5;

  // wraps automatically
  // Watch_H_Location += Watch_H_Location_Inc;

  Watch_H_Location_F += Watch_H_Location_Inc_F;

  Watch_H_Location = (int)(Watch_H_Location_F + .5);

  // Watch_H_Location = 0;

  // Fade_V_Location = 68;

  //    FIX THE MEMORY LEAKS ON THE NUC though there arent any on the desktop!!!!!!!!!!!!!!
  //    FIX THE MEMORY LEAKS ON THE NUC though there arent any on the desktop!!!!!!!!!!!!!!
  //    FIX THE MEMORY LEAKS ON THE NUC though there arent any on the desktop!!!!!!!!!!!!!!
  //    maybe do a non UMat_Type version

  Load_Time();

  // this is really slow on the NUC!!!
  // but only need to update the time once a minute or so
  // adds the hands to the watch

  if (D_Clock_Selected)
    Build_Clock(); // NUC 0 ms
  else
    Build_Watch(); // NUC 5 ms

  // timing tester
  // New_Timer.Start_Delay_Timer();
  // New_Timer.End_Delay_Timer();
  // cout << " New_Timer.time_delay  " << New_Timer.time_delay_avg << endl;

  // NUC .22 ms
  Shift_Image_Vertical_U2(AP1x.Alpha_Channel_FU, Alpha_Fade_Shifted_FU, Fade_V_Location, VP2x.Ones_Float_Mat_U);

  // multiply the 2 alphas the watch alpha and the fade alpha  // NUC .12 ms
  multiply(Alpha_Fade_Shifted_FU, VP2x.Alpha_Channel_Inv_FU, Combined_Alpha_Fade_FU);

  // multiply(Watch_With_Both, Combined_Alpha_Fade_FU, Watch_With_Both_Faded_U);

  // this shrinks the watch and its alpha , but puts them back in a full size Mat
  // NUC  .54 ms
  Shrink_Object(Watch_With_Both, Combined_Alpha_Fade_FU, VideoSum_Comp_FU, Alpha_Comp_FU, Watch_H_Size, Watch_V_Size);

  //  useful for isolating the scaling problem
  // UMat_Type Mat_Temp;
  // resize(VideoSum_Comp_FU, Mat_Temp, Size(), .25, .25, INTER_NEAREST);
  // resize(Mat_Temp, Mat_Temp, Size(), 4, 4, INTER_NEAREST);
  // blur(Mat_Temp, Mat_Temp, Size(6, 6));
  // resize(Mat_Temp, Mat_Temp, Size(), 4, 4, INTER_NEAREST);
  // Mat_Temp.convertTo(DisplayTemp, CV_8U, 1);

  // Watch_With_Both_Faded_U.convertTo(DisplayTemp, CV_8U);
  // imshow("17", DisplayTemp);

  // this rotates the watch and its alpha   // NUC 1.7 ms for both
  rotate2(VideoSum_Comp_FU, VideoSum_FUE, Watch_Angle);
  rotate2(Alpha_Comp_FU, Alpha_Rotated_U, Watch_Angle);

  // this moves the watch and its alpha horizontally and vertically (works out of frame also)  // NUC .5 ms for both
  Shift_Image_Horizontal_Vertical_U3(VideoSum_FUE, Watch_Shifted_FU, Watch_H_Location, (int)Watch_V_Location, VP2x.Zeros_Float_Mat_U);
  Shift_Image_Horizontal_Vertical_U3(Alpha_Rotated_U, Watch_Alpha_Shifted_FU, Watch_H_Location, (int)Watch_V_Location, VP2x.Zeros_Float_Mat_U);

  // add the fade into the water
  // NUC .22 ms
  Shift_Image_Vertical_U2(AP1x.Alpha_Channel_FU, Alpha_Fade_Shifted_Sinking_FU, 100, VP2x.Ones_Float_Mat_U);
  multiply(Alpha_Fade_Shifted_Sinking_FU, Watch_Alpha_Shifted_FU, Watch_Alpha_Shifted_FU_2);

  // generate the final watch matted image  // NUC .12 ms
  multiply(Watch_Shifted_FU, Watch_Alpha_Shifted_FU_2, Watch_With_Both_Faded_U);

  //   AP1x.Alpha_Channel_FU.convertTo(DisplayTemp, CV_8U,255);
  // imshow("17", DisplayTemp);

  // filters the shrunk watch and alpha  this needs to be done because shrinking the image creates sharper edges   // 1.3 ms for all
  blur(Watch_With_Both_Faded_U, VideoSum_FUE, Size(3, 3));
  blur(VideoSum_FUE, VideoSum_FUE, Size(5, 5));
  blur(VideoSum_FUE, VideoSum_FUE, Size(7, 7));
  blur(Watch_Alpha_Shifted_FU_2, Alpha_Rotated_U, Size(3, 3));
  blur(Alpha_Rotated_U, Alpha_Rotated_U, Size(5, 5));
  blur(Alpha_Rotated_U, Alpha_Rotated_U, Size(7, 7));

  //  create the inverted alpha for the background  // NUC  .075 ms
  subtract(VP2x.Ones_Float_Mat_U, Alpha_Rotated_U, Alpha_Rotated_U);

  //  soft key the final watch over the waves  // .21 ms for both
  multiply(VP1x.VideoProc_FU, Alpha_Rotated_U, VideoSum_FUF);
  addWeighted(VideoSum_FUE, 1, VideoSum_FUF, 1, 0, VideoSum_FU);

  if (!Watch_On)
    VideoSum_FU = VP1x.VideoProc_FU.clone();

  VideoSum_For_Output_FU = VideoSum_FU.clone();

  Start_Up_Gain += .005;
  if (Start_Up_Gain > 1)
    Start_Up_Gain = 1;
  Final_Output_Gain = Downstream_Gain * Sun_Gain * Start_Up_Gain;

  VideoSum_For_Output_FU *= Final_Output_Gain;
}

void Video_Sculpture::Multi_Map_Image_To_Sculpture(void)
{
  // .28 ms
  New_Timer.Start_Delay_Timer();
  // Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8( );
  Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8(VideoSum_For_Output_FU);
  New_Timer.End_Delay_Timer();
  cout << endl
       << "    New_Timer.time_delay  " << New_Timer.time_delay_avg << endl;

  if (Bypass_Output_Gain)
    Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8(VideoSum_FU, Sampled_Buffer_RGBW_View);
  else
    Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8(VideoSum_For_Output_FU, Sampled_Buffer_RGBW_View);

  // .065 ms
  Map_Subsampled_To_Sculpture();

  // .15 ms
  Generate_Subsampled_Image_For_Test();
}

// this was the fastet way that I measured
void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8(void)
{
  //  time_test.Start_Delay_Timer();
  int xx_arr, yy_arr, yy, xx;
  int inc = 0;
  uint16_t rz, gz, bz, wz, Sub_Valz;
  int i;

  Pixel_Type_F Pixel_Vec;

#ifdef UMat_Enable
  VideoSum_For_Output_FU.copyTo(VideoSum_F);
  VideoSum_F.convertTo(VideoSum_16, CV_16UC3, 256); // this does the clipping for you !!!
#else
  VideoSum_For_Output_FU.convertTo(VideoSum_16, CV_16UC3, 256);
#endif

  i = 0;

  if (Video_On)
  {
    for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) //
    {
      yy = 2 + (V_SAMPLE_SPREAD * yy_arr); //
      for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
      {
        xx = Sample_Points_Map_V[yy_arr][xx_arr];
        Pixel_Vec = VideoSum_16.at<Pixel_Type_16>(yy, xx);

        rz = Pixel_Vec[2];
        gz = Pixel_Vec[1];
        bz = Pixel_Vec[0];

        Sub_Valz = std::min({rz, gz, bz});

        Sampled_Buffer_RGBW[i++] = rz - Sub_Valz;
        Sampled_Buffer_RGBW[i++] = gz - Sub_Valz;
        Sampled_Buffer_RGBW[i++] = bz - Sub_Valz;
        Sampled_Buffer_RGBW[i++] = Sub_Valz;
      }
    }
  }

  else
  {
    for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) //
    {
      yy = 2 + (V_SAMPLE_SPREAD * yy_arr); //
      for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
      {
        Sampled_Buffer_RGBW[i++] = 0;
        Sampled_Buffer_RGBW[i++] = 0;
        Sampled_Buffer_RGBW[i++] = 0;
        Sampled_Buffer_RGBW[i++] = 0;
      }
    }
  }

  //  time_test.End_Delay_Timer();
  // cout << Sampled_Buffer_RGBW[0] / 256 << " ggg " << Sampled_Buffer_RGBW[1] / 256 << "  " << Sampled_Buffer_RGBW[2] / 256 << "  " << time_test.time_delay << endl;
}

// this was the fastet way that I measured
void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8(UMat_Type &Vid_Src)
{
  //  time_test.Start_Delay_Timer();
  int xx_arr, yy_arr, yy, xx;
  int inc = 0;
  uint16_t rz, gz, bz, wz, Sub_Valz;
  int i;

  Pixel_Type_F Pixel_Vec;

#ifdef UMat_Enable
  Vid_Src.copyTo(VideoSum_F);
  VideoSum_F.convertTo(VideoSum_16, CV_16UC3, 256); // this does the clipping for you !!!
#else
  Vid_Src.convertTo(VideoSum_16, CV_16UC3, 256);
#endif

  i = 0;

  if (Video_On)
  {
    for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) //
    {
      yy = 2 + (V_SAMPLE_SPREAD * yy_arr); //
      for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
      {
        xx = Sample_Points_Map_V[yy_arr][xx_arr];
        Pixel_Vec = VideoSum_16.at<Pixel_Type_16>(yy, xx);

        rz = Pixel_Vec[2];
        gz = Pixel_Vec[1];
        bz = Pixel_Vec[0];

        Sub_Valz = std::min({rz, gz, bz});

        Sampled_Buffer_RGBW[i++] = rz - Sub_Valz;
        Sampled_Buffer_RGBW[i++] = gz - Sub_Valz;
        Sampled_Buffer_RGBW[i++] = bz - Sub_Valz;
        Sampled_Buffer_RGBW[i++] = Sub_Valz;
      }
    }
  }

  else
  {
    for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) //
    {
      yy = 2 + (V_SAMPLE_SPREAD * yy_arr); //
      for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
      {
        Sampled_Buffer_RGBW[i++] = 0;
        Sampled_Buffer_RGBW[i++] = 0;
        Sampled_Buffer_RGBW[i++] = 0;
        Sampled_Buffer_RGBW[i++] = 0;
      }
    }
  }

  //  time_test.End_Delay_Timer();
  // cout << Sampled_Buffer_RGBW[0] / 256 << " ggg " << Sampled_Buffer_RGBW[1] / 256 << "  " << Sampled_Buffer_RGBW[2] / 256 << "  " << time_test.time_delay << endl;
}

// for test monitoring
void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev8(UMat_Type &Vid_Src, uint16_t *RGBW_Array)
{
  //  time_test.Start_Delay_Timer();
  int xx_arr, yy_arr, yy, xx;
  int inc = 0;
  uint16_t rz, gz, bz, wz, Sub_Valz;
  int i;

  Pixel_Type_F Pixel_Vec;

#ifdef UMat_Enable
  Vid_Src.copyTo(VideoSum_F);
  VideoSum_F.convertTo(VideoSum_16, CV_16UC3, 256); // this does the clipping for you !!!
#else
  Vid_Src.convertTo(VideoSum_16, CV_16UC3, 256);
#endif

  i = 0;

  if (Video_On)
  {
    for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) //
    {
      yy = 2 + (V_SAMPLE_SPREAD * yy_arr); //
      for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
      {
        xx = Sample_Points_Map_V[yy_arr][xx_arr];
        Pixel_Vec = VideoSum_16.at<Pixel_Type_16>(yy, xx);

        rz = Pixel_Vec[2];
        gz = Pixel_Vec[1];
        bz = Pixel_Vec[0];

        Sub_Valz = std::min({rz, gz, bz});

        RGBW_Array[i++] = rz - Sub_Valz;
        RGBW_Array[i++] = gz - Sub_Valz;
        RGBW_Array[i++] = bz - Sub_Valz;
        RGBW_Array[i++] = Sub_Valz;
      }
    }
  }

  else
  {
    for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) //
    {
      yy = 2 + (V_SAMPLE_SPREAD * yy_arr); //
      for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
      {
        RGBW_Array[i++] = 0;
        RGBW_Array[i++] = 0;
        RGBW_Array[i++] = 0;
        RGBW_Array[i++] = 0;
      }
    }
  }

  //  time_test.End_Delay_Timer();
  // cout << Sampled_Buffer_RGBW[0] / 256 << " ggg " << Sampled_Buffer_RGBW[1] / 256 << "  " << Sampled_Buffer_RGBW[2] / 256 << "  " << time_test.time_delay << endl;
}

// formerly known as Panel_Mapper
// maps the image sub samples to the panel using the panel map
void Video_Sculpture::Map_Subsampled_To_Sculpture(void)
{
  for (int ii = 0; ii < Sculpture_Size_Pixels; ii++)
  {
    int iixx = Words_Per_Pixel * ii;
    int Panel_MapxX = Words_Per_Pixel * Sculpture_Map[ii];
    for (int jj = 0; jj < Words_Per_Pixel; jj++)
      Samples_Mapped_To_Sculpture[Panel_MapxX + jj] = Sampled_Buffer_RGBW[iixx + jj];
  }
}

// just started
// void Video_Sculpture::Generate_Subsampled_Image_For_Test(void)
// {

//   static uchar r, g, b, w;
//   static int i, o;

//   int inc;
//   i = 0;
//   o = 0;

//   for (inc = 0; inc < 3600; inc++)
//   {
//     r = Sampled_Buffer_RGBW[i++] / 256;
//     g = Sampled_Buffer_RGBW[i++] / 256;
//     b = Sampled_Buffer_RGBW[i++] / 256;
//     w = Sampled_Buffer_RGBW[i++] / 256;

//     r = r + w;
//     g = g + w;
//     b = b + w;

//     Sampled_Buffer_RGB[o++] = b;
//     Sampled_Buffer_RGB[o++] = g;
//     Sampled_Buffer_RGB[o++] = r;
//   }

//   std::memcpy(Subsampled_Display_Small.data, Sampled_Buffer_RGB, 50 * 72 * 3 * sizeof(uchar));

//   // imshow("ouput", Subsampled_Display_Small);

//   resize(Subsampled_Display_Small, Subsampled_Display_Large, Size(), 5, 5, INTER_LINEAR);
// }

void Video_Sculpture::Generate_Subsampled_Image_For_Test(void)
{

  static uchar r, g, b, w;
  static int i, o;

  int inc;
  i = 0;
  o = 0;

  for (inc = 0; inc < 3600; inc++)
  {
    r = Sampled_Buffer_RGBW_View[i++] / 256;
    g = Sampled_Buffer_RGBW_View[i++] / 256;
    b = Sampled_Buffer_RGBW_View[i++] / 256;
    w = Sampled_Buffer_RGBW_View[i++] / 256;

    r = r + w;
    g = g + w;
    b = b + w;

    Sampled_Buffer_RGB[o++] = b;
    Sampled_Buffer_RGB[o++] = g;
    Sampled_Buffer_RGB[o++] = r;
  }

  std::memcpy(Subsampled_Display_Small.data, Sampled_Buffer_RGB, 50 * 72 * 3 * sizeof(uchar));

  // imshow("ouput", Subsampled_Display_Small);

  resize(Subsampled_Display_Small, Subsampled_Display_Large, Size(), 5, 5, INTER_LINEAR);
}

void Video_Sculpture::Add_Visible_Sample_Locations_From_Sample_Points_Map(cv::Mat src) // , int Display_Type)
{

  int Display_Type = 1;

  int xx_arr, yy_arr;
  int rowsY = src.rows;
  int colsX = src.cols;
  Pixel_Type Pixel_Vec;
  Vec3s vv;

  time_test.Start_Delay_Timer();

  for (int yy = 2; (yy < rowsY); yy += V_SAMPLE_SPREAD)
  {
    xx_arr = 0;
    yy_arr = (yy - 2) >> 2;
    for (int xx = 0; (xx < colsX); xx++)
    {
      if ((yy_arr < Sample_Points_Map_V.size()) && (xx_arr < Sample_Points_Map_V[yy_arr].size()))
        if (Sample_Points_Map_V[yy_arr][xx_arr] == xx)
        {
          Pixel_Vec = src.at<Vec3b>(Point(xx, yy));
          uint8_t R0 = (Pixel_Vec[0] <= 100) ? Pixel_Vec[0] + 100 : 0;
          uint8_t G0 = (Pixel_Vec[1] <= 100) ? Pixel_Vec[1] + 100 : 0;
          uint8_t B0 = (Pixel_Vec[2] <= 100) ? Pixel_Vec[2] + 100 : 0;

          if (Display_Type == 1)
            src.at<Vec3b>(Point(xx, yy)) = {0, 0, 0};
          else
            src.at<Vec3b>(Point(xx, yy)) = {R0, G0, B0};
          xx_arr++;
        }
    }
  }

  time_test.End_Delay_Timer();
  cout << " lower " << time_test.time_delay << endl;
}

void Video_Sculpture::Video_Sculpture::Add_Visible_Sample_Locations_From_Sample_Points_Map_Ver2(cv::Mat src)
{
  int xx_arr, yy_arr, yy, xx;
  Pixel_Type Pixel_Vec;

  time_test.Start_Delay_Timer();
  for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) // up to 67
  {
    yy = 2 + (V_SAMPLE_SPREAD * yy_arr); // up to 280
    for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
    {
      xx = Sample_Points_Map_V[yy_arr][xx_arr];

      Pixel_Vec = src.at<Vec3b>(Point(xx, yy));
      // uint8_t R0 = (Pixel_Vec[0] <= 100) ? Pixel_Vec[0] + 100 : 0;
      // uint8_t G0 = (Pixel_Vec[1] <= 100) ? Pixel_Vec[1] + 100 : 0;
      // uint8_t B0 = (Pixel_Vec[2] <= 100) ? Pixel_Vec[2] + 100 : 0;
      uint8_t R0 = Pixel_Vec[0] + 100;
      uint8_t G0 = Pixel_Vec[1] + 100;
      uint8_t B0 = Pixel_Vec[2] + 100;

      src.at<Vec3b>(Point(xx, yy)) = {R0, G0, B0};
    }
  }
  time_test.End_Delay_Timer();
  cout << " lower2 " << time_test.time_delay << endl;
}

void Video_Sculpture::Display(void)
{
  // quicker as a 2 step convert

#ifdef UMat_Enable
  VideoSum_FU.convertTo(VideoSum_U, CV_8UC3);     // convert a UMat_Type float to a UMat_Type int
  VideoSum_U.convertTo(VideoSumDisplay, CV_8UC3); // convert a UMat_Type int to a Mat int
#else
  VideoSum_FU.convertTo(VideoSumDisplay, CV_8UC3);
#endif

  //  Add_Visible_Sample_Locations_From_Sample_Points_Map_Ver2(VideoSumDisplay);

  namedWindow("Sum", WINDOW_AUTOSIZE); // Create a window for display.
  imshow("Sum", VideoSumDisplay);      // Show our image inside it.

  Display_Text_Mat("values", text_window, 50, 50);

  imshow("ouput", Subsampled_Display_Large);

  if (display_on_X)
  {
    imshow("1", VP1x.VideoDisplay);
    imshow("2", VP2x.VideoDisplay);
    imshow("3", VP3x.VideoDisplay);
  }
  else
  {
    if (cv::getWindowProperty("1", WND_PROP_AUTOSIZE) != -1)
      destroyWindow("1");
    if (cv::getWindowProperty("2", WND_PROP_AUTOSIZE) != -1)
      destroyWindow("2");
    if (cv::getWindowProperty("3", WND_PROP_AUTOSIZE) != -1)
      destroyWindow("3");
    //   if (cv::getWindowProperty("output", WND_PROP_AUTOSIZE) != -1)
    //     destroyWindow("output");
  }
}

// this was the fastet way that I measured
// overflows
// void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev6(void)
// {
//   time_test.Start_Delay_Timer();
//   int xx_arr, yy_arr, yy, xx;
//   int inc = 0;
//   uint16_t rz, gz, bz, wz, Sub_Valz;

//   Pixel_Type_F Pixel_Vec;

//   VideoSum_FU.copyTo(VideoSum_F);

//   // VideoSum_F = cv::Scalar(0,0,0)

//   int i = 0;
//   for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) //
//   {
//     yy = 2 + (V_SAMPLE_SPREAD * yy_arr); //
//     for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
//     {
//       xx = Sample_Points_Map_V[yy_arr][xx_arr];
//       Pixel_Vec = VideoSum_F.at<Pixel_Type_F>(yy, xx);

//       rz = (Pixel_Vec[2] < 0) ? 0 : (uint16_t)(256 * Pixel_Vec[2]);
//       gz = (Pixel_Vec[1] < 0) ? 0 : (uint16_t)(256 * Pixel_Vec[1]);
//       bz = (Pixel_Vec[0] < 0) ? 0 : (uint16_t)(256 * Pixel_Vec[0]);

//       // if ((rz <= gz) & (rz <= bz))
//       //   Sub_Valz = rz;
//       // else if ((gz <= rz) & (gz <= bz))
//       //   Sub_Valz = gz;
//       // else
//       //   Sub_Valz = bz;

//       Sub_Valz = std::min({rz, gz, bz});

//       Sampled_Buffer_RGBW[i++] = rz - Sub_Valz;
//       Sampled_Buffer_RGBW[i++] = gz - Sub_Valz;
//       Sampled_Buffer_RGBW[i++] = bz - Sub_Valz;
//       Sampled_Buffer_RGBW[i++] = Sub_Valz;
//     }
//   }
//   time_test.End_Delay_Timer();
//   cout << Sampled_Buffer_RGBW[0] / 256 << " ggg " << Sampled_Buffer_RGBW[1] / 256 << "  " << Sampled_Buffer_RGBW[2] / 256 << "  " << time_test.time_delay << endl;
// }

// this was the fastet way that I measured
// slower than the conversion U16C3 in Rev8
// void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev7(void)
// {
//   time_test.Start_Delay_Timer();
//   int xx_arr, yy_arr, yy, xx;
//   int inc = 0;
//   uint16_t rz, gz, bz, wz, Sub_Valz;

//   Pixel_Type_F Pixel_Vec;

//   VideoSum_FU.copyTo(VideoSum_F);
//   VideoSum_F.forEach<Pixel3F>(Operator());

//   int i = 0;
//   for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) //
//   {
//     yy = 2 + (V_SAMPLE_SPREAD * yy_arr); //
//     for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
//     {
//       xx = Sample_Points_Map_V[yy_arr][xx_arr];
//       Pixel_Vec = VideoSum_F.at<Pixel_Type_F>(yy, xx);

//       rz = Pixel_Vec[2];
//       gz = Pixel_Vec[1];
//       bz = Pixel_Vec[0];

//       Sub_Valz = std::min({rz, gz, bz});

//       Sampled_Buffer_RGBW[i++] = rz - Sub_Valz;
//       Sampled_Buffer_RGBW[i++] = gz - Sub_Valz;
//       Sampled_Buffer_RGBW[i++] = bz - Sub_Valz;
//       Sampled_Buffer_RGBW[i++] = Sub_Valz;
//     }
//   }

//   time_test.End_Delay_Timer();
//   cout << Sampled_Buffer_RGBW[0] / 256 << " ggg " << Sampled_Buffer_RGBW[1] / 256 << "  " << Sampled_Buffer_RGBW[2] / 256 << "  " << time_test.time_delay << endl;
// }

// this was the fastet way that I measured
// HAD OVERFLOW PROBLEM !!!!!!!!!!!!!!!!!!!!!!!!1
// void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev5(void)
// {
//   float r, g, b, w, Sub_Val;
//   int xx_arr, yy_arr, yy, xx;
//   int inc = 0;

//   time_test.Start_Delay_Timer();

//   Pixel_Type_F Pixel_Vec;

//   VideoSum_FU.copyTo(VideoSum_F);

//   for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) // up to 67
//   {
//     yy = 2 + (V_SAMPLE_SPREAD * yy_arr); // up to 280
//     for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
//     {
//       xx = Sample_Points_Map_V[yy_arr][xx_arr];
//       Pixel_Vec = VideoSum_F.at<Pixel_Type_F>(yy, xx);
//       r = Pixel_Vec[2];
//       g = Pixel_Vec[1];
//       b = Pixel_Vec[0];

//       if (g < 0)
//         cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX SUBVAL " << g << endl;
//       // if (b > 100) cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX BBBBBBBBBBBB "  << (int)b << endl ;

//       if ((r <= g) & (r <= b))
//         Sub_Val = r;
//       else if ((g <= r) & (g <= b))
//         Sub_Val = g;
//       else
//         Sub_Val = b;

//       *(Sampled_Buffer_RGBW_AF + inc++) = r - Sub_Val;
//       *(Sampled_Buffer_RGBW_AF + inc++) = g - Sub_Val;
//       *(Sampled_Buffer_RGBW_AF + inc++) = b - Sub_Val;
//       *(Sampled_Buffer_RGBW_AF + inc++) = Sub_Val;

//       if (Sub_Val < 0)
//         cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX SUBVAL 555555555555555555" << endl;
//     }
//   }

//   for (int i = 0; i < Sculpture_Size_RGBW; i++)
//     Sampled_Buffer_RGBW[i] = (uint16_t)(256 * Sampled_Buffer_RGBW_AF[i]);

//   time_test.End_Delay_Timer();

//   cout << Sampled_Buffer_RGBW[0] / 256 << " ggg " << Sampled_Buffer_RGBW[1] / 256 << "  " << Sampled_Buffer_RGBW[2] / 256 << "  " << time_test.time_delay << endl;
// }

// void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_V_Float(cv::Mat src, float *Buffer, const vector<vector<int>> &SP)
// {
//   float r, g, b, w, Sub_Val;
//   int xx_arr, yy_arr, yy, xx;
//   int inc = 0;
//   Pixel_Type_F Pixel_Vec;
//   for (yy_arr = 0; (yy_arr < SP.size()); yy_arr++) // up to 67
//   {
//     yy = 2 + (4 * yy_arr); // up to 280
//     for (xx_arr = 0; xx_arr < SP[yy_arr].size(); xx_arr++)
//     {
//       xx = SP[yy_arr][xx_arr];
//       Pixel_Vec = src.at<Pixel_Type_F>(yy, xx);
//       r = Pixel_Vec[2];
//       g = Pixel_Vec[1];
//       b = Pixel_Vec[0];

//       if ((r <= g) & (r <= b))
//         Sub_Val = r;
//       else if ((g <= r) & (g <= b))
//         Sub_Val = g;
//       else
//         Sub_Val = b;

//       *(Buffer + inc++) = r - Sub_Val;
//       *(Buffer + inc++) = g - Sub_Val;
//       *(Buffer + inc++) = b - Sub_Val;
//       *(Buffer + inc++) = Sub_Val;
//     }
//   }
// }

// these versions were slower!! except for the 1st one
//    which assumes that all the points are on a grid ( though not all used )

// // version 0 subsample the video 1st  this only works if all of the sample end up on the sub-grid  much faster
// resize(VideoSum_FU, VideoSum_Small_FU, Size(), .25, .25, INTER_NEAREST);
// VideoSum_Small_FU.convertTo(VideoSum_Small_16, CV_16UC3, 256);
// Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Small(VideoSum_Small_16, Grabed_Buffer_RGBW_A, Sample_Points_Map_A, Num_Of_Samples_Per_Row);
// cout << Grabed_Buffer_RGBW_A[0] / 256 << "  " << Grabed_Buffer_RGBW_A[1] / 256 << "  " << Grabed_Buffer_RGBW_A[2] / 256 << endl;

// //version 1 non vectored  Sample_Points_Map_A
// //convert from UMat_Type to Mat
// VideoSum_FU.copyTo(VideoSum_F);
// //convert from Float to 16 bit unsigned
// VideoSum_F.convertTo(VideoSum_16, CV_16UC3, 256);
// Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert(VideoSum_16, Grabed_Buffer_RGBW_A, Sample_Points_Map_A, Num_Of_Samples_Per_Row);
// cout << Grabed_Buffer_RGBW_A[0] / 256 << "  " << Grabed_Buffer_RGBW_A[1] / 256 << "  " << Grabed_Buffer_RGBW_A[2] / 256 << endl;

// // version 2 vectored  Sample_Points_Map_V
// // convert from UMat_Type to Mat
// VideoSum_FU.copyTo(VideoSum_F);
// //convert from Float to 16 bit unsigned
// VideoSum_F.convertTo(VideoSum_16, CV_16UC3, 256);
// Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_V(VideoSum_16, Grabed_Buffer_RGBW_B, Sample_Points_Map_V);
// cout << Grabed_Buffer_RGBW_B[0] / 256 << "  " << Grabed_Buffer_RGBW_B[1] / 256 << "  " << Grabed_Buffer_RGBW_B[2] / 256 << endl;

// void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Small(cv::Mat src, uint16_t *Buffer, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS])
// {
//   uint16_t r, g, b, w, Sub_Val;
//   int xx_arr, yy_arr, yy, xx;
//   int inc = 0;
//   Pixel_Type Pixel_Vec;
//   for (yy_arr = 0; (yy_arr < SAMPLE_ROWS); yy_arr++) // up to 67
//   {
//     yy = yy_arr; // up to 280
//     for (xx_arr = 0; xx_arr < Num_Of_Samples_Per_Row[yy_arr]; xx_arr++)
//     {
//       xx = SP[yy_arr][xx_arr] / 4;
//       Pixel_Vec = src.at<Pixel_Type>(yy, xx);
//       r = Pixel_Vec[2];
//       g = Pixel_Vec[1];
//       b = Pixel_Vec[0];

//       if ((r <= g) & (r <= b))
//         Sub_Val = r;
//       else if ((g <= r) & (g <= b))
//         Sub_Val = g;
//       else
//         Sub_Val = b;

//       *(Buffer + inc++) = r - Sub_Val;
//       *(Buffer + inc++) = g - Sub_Val;
//       *(Buffer + inc++) = b - Sub_Val;
//       *(Buffer + inc++) = Sub_Val;
//     }
//   }
// }

// void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_V(cv::Mat src, uint16_t *Buffer, const vector<vector<int>> &SP)
// {
//   uint16_t r, g, b, w, Sub_Val;
//   int xx_arr, yy_arr, yy, xx;
//   int inc = 0;
//   Pixel_Type Pixel_Vec;
//   for (yy_arr = 0; (yy_arr < SP.size()); yy_arr++) // up to 67
//   {
//     yy = 2 + (4 * yy_arr); // up to 280
//     for (xx_arr = 0; xx_arr < SP[yy_arr].size(); xx_arr++)
//     {
//       xx = SP[yy_arr][xx_arr];
//       Pixel_Vec = src.at<Pixel_Type>(yy, xx);
//       r = Pixel_Vec[2];
//       g = Pixel_Vec[1];
//       b = Pixel_Vec[0];

//       if ((r <= g) & (r <= b))
//         Sub_Val = r;
//       else if ((g <= r) & (g <= b))
//         Sub_Val = g;
//       else
//         Sub_Val = b;

//       *(Buffer + inc++) = r - Sub_Val;
//       *(Buffer + inc++) = g - Sub_Val;
//       *(Buffer + inc++) = b - Sub_Val;
//       *(Buffer + inc++) = Sub_Val;
//     }
//   }
// }

// void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert(cv::Mat src, uint16_t *Buffer, int SP[67][256], int *Num_Of_Samples_Per_Row)
// {
//   uint16_t r, g, b, w, Sub_Val;
//   int xx_arr, yy_arr, yy, xx;
//   int inc = 0;
//   Pixel_Type Pixel_Vec;
//   for (yy_arr = 0; (yy_arr < SAMPLE_ROWS); yy_arr++) // up to 67
//   {
//     yy = 2 + (4 * yy_arr); // up to 280
//     for (xx_arr = 0; xx_arr < Num_Of_Samples_Per_Row[yy_arr]; xx_arr++)
//     {
//       xx = SP[yy_arr][xx_arr];
//       Pixel_Vec = src.at<Pixel_Type>(yy, xx);
//       r = Pixel_Vec[2];
//       g = Pixel_Vec[1];
//       b = Pixel_Vec[0];

//       if ((r <= g) & (r <= b))
//         Sub_Val = r;
//       else if ((g <= r) & (g <= b))
//         Sub_Val = g;
//       else
//         Sub_Val = b;

//       *(Buffer + inc++) = r - Sub_Val;
//       *(Buffer + inc++) = g - Sub_Val;
//       *(Buffer + inc++) = b - Sub_Val;
//       *(Buffer + inc++) = Sub_Val;
//     }
//   }
// }
// ALSO  FIX THE MEMORY LEAKS ON THE NUC though there arent any on the desktop!!!!!!!!!!!!!!