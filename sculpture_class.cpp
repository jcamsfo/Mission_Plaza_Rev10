

#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <thread>
#include <iomanip>

#include "sculpture_class.h"
#include "player_class.h"
#include "measure2.h"
#include "process_other.h"
#include "file_io_2.h"

using namespace std::chrono;
using namespace std;
using namespace cv;

// #define Small_Hand_Image VP3x.VideoProc_FU

typedef Vec<uint8_t, 3> Pixel_Type_8b;

typedef Vec<uint16_t, 3> Pixel_Type;

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

inline void rotate2(UMat &src, UMat &dst, double angle) //rotate function returning mat object with parametres imagefile and angle
{
  // static int Rotating_Angle;

  // Rotating_Angle++;
  // if (Rotating_Angle >= 360)
  //   Rotating_Angle = 0;
  Point2f pt(src.cols / 2., src.rows / 2.);          //point from where to rotate
  Mat r = getRotationMatrix2D(pt, angle, 1.0);       //Mat object for storing after rotation
  warpAffine(src, dst, r, Size(src.cols, src.rows)); ///applie an affine transforation to image.
}

inline void Overlay(UMat &Frgnd, UMat &Bkgnd, UMat &Alpha, UMat &Result)
{
  static UMat Temp;
  multiply(Bkgnd, Alpha, Temp);
  addWeighted(Temp, 1, Frgnd, 1, 0, Result);
}

Video_Sculpture::Video_Sculpture(void)
{

  Sampled_Buffer_RGBW = new uint16_t[Sculpture_Size_RGBW]; //  16 bit   4 * 13116  52464
  Sampled_Buffer_RGBW_AF = new float[Sculpture_Size_RGBW]; //  16 bit   4 * 13116  52464
  Samples_Mapped_To_Sculpture = new uint16_t[Buffer_W_Gaps_Size_RGBW_Extra];

  // pictures for the tower
  // VP1x.setup("../../Movies/TSB3X.mov", "0");
  // VP2x.setup("../../Movies/flagblack.mov", "1");

  // VP1x.setup("../../Movies/rainbow.mp4", "0", 1);
  // VP2x.VideoSetup("../../Movies/comp4_264.mov", "1");
  // VP3x.StillSetup("../../Movies/alpha_trans.tif", "3");

  VP1x.VideoSetup("../../Movies/waterloop250.mov", "0");
  //VP2x.StillSetupRev2("../../Movies/watch_SM.tif", "1");
  VP2x.StillSetupWithAlpha("../../Movies/wtc3s.tif", "1");
  VP3x.StillSetupWithAlpha("../../Movies/smallhand.tif", "2");
  VP4x.StillSetupWithAlpha("../../Movies/bighand.tif", "3");

  AP1x.AlphaSetupNoProcess("../../Movies/fader_soft_3C.tif", "4");

  // CODING KEY:   F -> float type (vs unsigned char)     U ->  UMat (vs Mat)
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

  display_on_X = true;

  local_oop = 0;
  Rotating_Angle = 360;
  Rotating_Angle_inc = 1;
  shrink_val = 0.5;
  shrink_val_inc = .01;
  X_Position = 0;
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
  Watch_Alpha = VP2x.Alpha_Channel_FU;

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

inline void Video_Sculpture::Shrink_Object(UMat &src, UMat &src_alpha, UMat &dst, UMat &dst_alpha, double scale_factor_h, double scale_factor_v)
{
  static UMat resized;
  static UMat resized_alpha;
  static int x, y;
  resize(src, resized, Size(), scale_factor_h, scale_factor_v, INTER_LINEAR);
  x = (src.cols - resized.cols) / 2;
  y = (src.rows - resized.rows) / 2;
  dst = VP2x.Zeros_Float_Mat_U.clone();
  resized.copyTo(dst(cv::Rect(x, y, resized.cols, resized.rows)));
  resize(src_alpha, resized_alpha, Size(), scale_factor_h, scale_factor_v, INTER_LINEAR);
  dst_alpha = VP2x.Zeros_Float_Mat_U.clone();
  resized_alpha.copyTo(dst_alpha(cv::Rect(x, y, resized_alpha.cols, resized_alpha.rows)));
}





void Video_Sculpture::Play_All(void)
{

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
  // Rotating_Angle++;
  // if (Rotating_Angle >= 360)
  //   Rotating_Angle = 0;

  if (Rotating_Angle >= 370)
    Rotating_Angle_inc = -.2;
  else if (Rotating_Angle <= 350)
    Rotating_Angle_inc = .2;
  Rotating_Angle += Rotating_Angle_inc;

  if (shrink_val >= .6)
    shrink_val_inc = -.001;
  else if (shrink_val <= .5)
    shrink_val_inc = .001;
  shrink_val += shrink_val_inc;

  X_Position += 3;

  //    FIX THE MEMORY LEAKS ON THE NUC though there arent any on the desktop!!!!!!!!!!!!!!
  //    FIX THE MEMORY LEAKS ON THE NUC though there arent any on the desktop!!!!!!!!!!!!!!
  //    FIX THE MEMORY LEAKS ON THE NUC though there arent any on the desktop!!!!!!!!!!!!!!
  //    maybe do a non UMat version

  Load_Time();

  // this is really slow on the NUC!!!
  // but only need to update the time once a minute or so
  // adds the hands to the watch   
  Build_Watch();
  // check tduration of Build
  // Watch_With_Both = VP2x.VideoProc_FU.clone();


  // works 
  // Shift_Image_Vertical_U(Alpha_Fade_Shifted_FU, 50, VP2x.Ones_Float_Mat_U);
  // seems faster
  // set the location of the watch vertical fader
  Shift_Image_Vertical_U2(AP1x.Alpha_Channel_FU, Alpha_Fade_Shifted_FU, 40,  VP2x.Ones_Float_Mat_U);

  // multiply the 2 alphas the watch alpha and the fade alpha
  multiply(Alpha_Fade_Shifted_FU,  VP2x.Alpha_Channel_Inv_FU, Alpha_Fade_FU);
  // generate the final watch matted image
  multiply(Watch_With_Both, Alpha_Fade_Shifted_FU, Watch_With_Both_Faded_U);
  

  // this shrinks the watch and its alpha , but puts them back in a full size Mat
  // Shrink_Object(Watch_With_Both, Alpha_Fade_FU, VideoSum_Comp_FU, Alpha_Comp_FU, shrink_val, .5);
  Shrink_Object(Watch_With_Both_Faded_U, Alpha_Fade_FU, VideoSum_Comp_FU, Alpha_Comp_FU, shrink_val, .5);

  // this rotates the watch and its alpha
  rotate2(VideoSum_Comp_FU, VideoSum_FUE, Rotating_Angle);
  rotate2(Alpha_Comp_FU, Alpha_Rotated_U, Rotating_Angle);

    // this moves the watch and its alpha horizontally and vertically
  Shift_Image_Horizontal_Vertical_U2(VideoSum_FUE, Watch_Shifted_FU, X_Position, 100, VP2x.Zeros_Float_Mat_U);
  Shift_Image_Horizontal_Vertical_U2(Alpha_Rotated_U, Watch_Alpha_Shifted_FU, X_Position, 100, VP2x.Zeros_Float_Mat_U);


  Watch_Alpha_Shifted_FU.convertTo(DisplayTemp, CV_8U, 255);
  // Watch_With_Both_Faded_U.convertTo(DisplayTemp, CV_8U);  
  imshow("17", DisplayTemp);

  // filters the shrunk watch and alpha  this needs to be done because shrinking the image creates sharper edges
  blur(Watch_Shifted_FU, VideoSum_FUE, Size(3, 3));
  blur(VideoSum_FUE, VideoSum_FUE, Size(5, 5));
  blur(Watch_Alpha_Shifted_FU, Alpha_Rotated_U, Size(3, 3));
  blur(Alpha_Rotated_U, Alpha_Rotated_U, Size(5, 5));

//  create the inverted alpha for the background
  subtract(VP2x.Ones_Float_Mat_U, Alpha_Rotated_U, Alpha_Rotated_U);

//  soft key the final watch over the waves
  multiply(VP1x.VideoProc_FU, Alpha_Rotated_U, VideoSum_FUF);
  addWeighted(VideoSum_FUE, 1, VideoSum_FUF, 1, 0, VideoSum_FU);
}




void Video_Sculpture::Display(void)
{
  // quicker as a 2 step convert
  VideoSum_FU.convertTo(VideoSum_U, CV_8UC3);     // convert a UMAt float to a UMat int
  VideoSum_U.convertTo(VideoSumDisplay, CV_8UC3); // convert a UMAt int to a Mat int

  Add_Visible_Sample_Locations_From_Sample_Points_Map_Ver2(VideoSumDisplay);

  namedWindow("Sum", WINDOW_AUTOSIZE); // Create a window for display.
  imshow("Sum", VideoSumDisplay);      // Show our image inside it.

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
  }
}


void Video_Sculpture::Multi_Map_Image_To_Sculpture(void)
{
  Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev5();
  Map_Subsampled_To_Sculpture();
}



// this was the fastet way that I measured
void Video_Sculpture::Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev5(void)
{
  float r, g, b, w, Sub_Val;
  int xx_arr, yy_arr, yy, xx;
  int inc = 0;

  time_test.Start_Delay_Timer();

  Pixel_Type_F Pixel_Vec;

  VideoSum_FU.copyTo(VideoSum_F);

  for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) // up to 67
  {
    yy = 2 + (V_SAMPLE_SPREAD * yy_arr); // up to 280
    for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)
    {
      xx = Sample_Points_Map_V[yy_arr][xx_arr];
      Pixel_Vec = VideoSum_F.at<Pixel_Type_F>(yy, xx);
      r = Pixel_Vec[2];
      g = Pixel_Vec[1];
      b = Pixel_Vec[0];

      if ((r <= g) & (r <= b))
        Sub_Val = r;
      else if ((g <= r) & (g <= b))
        Sub_Val = g;
      else
        Sub_Val = b;

      *(Sampled_Buffer_RGBW_AF + inc++) = r - Sub_Val;
      *(Sampled_Buffer_RGBW_AF + inc++) = g - Sub_Val;
      *(Sampled_Buffer_RGBW_AF + inc++) = b - Sub_Val;
      *(Sampled_Buffer_RGBW_AF + inc++) = Sub_Val;
    }
  }

  time_test.End_Delay_Timer();

  for (int i = 0; i < Sculpture_Size_RGBW; i++)
    Sampled_Buffer_RGBW[i] = 256 * Sampled_Buffer_RGBW_AF[i];

  cout << Sampled_Buffer_RGBW[0] / 256 << " ggg " << Sampled_Buffer_RGBW[1] / 256 << "  " << Sampled_Buffer_RGBW[2] / 256 << "  " << time_test.time_delay << endl;
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
void Video_Sculpture::Generate_Subsampled_Image_For_Test(uint16_t *Buffer, bool RGBW_or_RGB, vector<vector<int>> X_Sample_Points, int Y_Start, int X_Increment, int X_Start, int Y_Increment)
{
  Mat Display_Mat;
  Pixel_Type a;
  a[0] = 3;
  Display_Mat.at<Pixel_Type>(0, 0) = a;
  // Mat_Out.at<Pixel_Type>(yy, xx) = a;
}

// for (yy_arr = 0; (yy_arr < Sample_Points_Map_V.size()); yy_arr++) // up to 67
// {
//   yy = 2 + (4 * yy_arr); // up to 280
//   for (xx_arr = 0; xx_arr < Sample_Points_Map_V[yy_arr].size(); xx_arr++)

// void Add_Visible_Sample_Locations_From_Sample_Points_Map(cv::Mat src, int SP[SAMPLE_ROWS][SAMPLE_COLS], int Num_Of_Samples_Per_Row[SAMPLE_ROWS], int Display_Type)
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
// //convert from UMat to Mat
// VideoSum_FU.copyTo(VideoSum_F);
// //convert from Float to 16 bit unsigned
// VideoSum_F.convertTo(VideoSum_16, CV_16UC3, 256);
// Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert(VideoSum_16, Grabed_Buffer_RGBW_A, Sample_Points_Map_A, Num_Of_Samples_Per_Row);
// cout << Grabed_Buffer_RGBW_A[0] / 256 << "  " << Grabed_Buffer_RGBW_A[1] / 256 << "  " << Grabed_Buffer_RGBW_A[2] / 256 << endl;

// // version 2 vectored  Sample_Points_Map_V
// // convert from UMat to Mat
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