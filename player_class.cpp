

#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

#include "player_class.h"
#include "measure2.h"
#include "process_other.h"

using namespace std::chrono;
using namespace std;
using namespace cv;


void RGB_Gamma_Correction(cv::UMat & src, float Gamma_Correction )
{
    UMat src_squared;
    multiply(src,src,src_squared);  // square
    addWeighted( src, 1 - Gamma_Correction , src_squared, .0039*Gamma_Correction, 0.0, src);
}


Video_Player_With_Processing::Video_Player_With_Processing(void){

};

Video_Player_With_Processing::Video_Player_With_Processing(string File_Name, string NameX)
{

  cout << " File_Name PC " << File_Name  << endl;

  capMain.open(File_Name);
  capWidth = capMain.get(CAP_PROP_FRAME_WIDTH);
  capHeight = capMain.get(CAP_PROP_FRAME_HEIGHT);
  capLength = capMain.get(cv::CAP_PROP_FRAME_COUNT);

  VideoMain.create(capHeight, capWidth, CV_8UC(3));
  VideoMain_U.create(capHeight, capWidth, CV_8UC(3));
  VideoMain_FU.create(capHeight, capWidth, CV_32FC(3));
  // VideoDisplay.create(capHeight, capWidth, CV_8UC(3));
  // VideoProc_FU.create(capHeight, capWidth, CV_8UC(3));

  Color_Difference_FU3.create(capHeight, capWidth, CV_8UC(3));
  Y_FU1.create(capHeight, capWidth, CV_8UC(1));
  VideoTemp_FU3.create(capHeight, capWidth, CV_8UC(3));

  player_pause = false;

  display_name = NameX;

  display_on = false;

  Ones2x2_A = false;
  Ones3x3_A = true;
  Ones4x4_A = false;
  Ones5x5_A = true;
  Ones6x6_A = false;
  Ones7x7_A = false;

  Gain = 1.;
  Black_Level = 0;
  Color_Gain = 2;

  // figure out where to put this as it's for the building not so much for the picture
  // like color correction
  Image_Gamma = 0;

  Prog_Durations DelX;
};

void Video_Player_With_Processing::setup(string File_Name, string NameX)
{




  capMain.open(File_Name);
  capWidth = capMain.get(CAP_PROP_FRAME_WIDTH);
  capHeight = capMain.get(CAP_PROP_FRAME_HEIGHT);
  capLength = capMain.get(cv::CAP_PROP_FRAME_COUNT);




  // KEY:   F -> float type (vs unsigned char)     U ->  UMat (vs Mat)

  VideoMain.create(capHeight, capWidth, CV_8UC(3));
  VideoMain_U.create(capHeight, capWidth, CV_8UC(3));
  VideoMain_FU.create(capHeight, capWidth, CV_32FC(3));
 

  VideoDisplay.create(capHeight, capWidth, CV_8UC(3));
  VideoProc_FU.create(capHeight, capWidth, CV_8UC(3));

  Color_Difference_FU3.create(capHeight, capWidth, CV_8UC(3));
  Y_FU1.create(capHeight, capWidth, CV_8UC(1));
  VideoTemp_FU3.create(capHeight, capWidth, CV_8UC(3));

  player_pause = false;

  display_name = NameX;

  Ones2x2_A = false;
  Ones3x3_A = true;
  Ones4x4_A = false;
  Ones5x5_A = true;
  Ones6x6_A = false;
  Ones7x7_A = false;

  Gain = 1.;
  Black_Level = 0;
  Color_Gain = 2;

  // figure out where to put this as it's for the building not so much for the picture
  // like color correction
  Image_Gamma = 0;


};



void Video_Player_With_Processing::Process(void)
{
  if (!player_pause)
  {
    capMain >> VideoMain;
    // read the video file
    if (capMain.get(cv::CAP_PROP_POS_FRAMES) == capLength)
      capMain.set(cv::CAP_PROP_POS_FRAMES, 0);

    // Change_Seam(img_in_A, 1);  not a UMat function
     Shift_Image_Horizontal(VideoMain, 180);
  }


  // convert to UMat for faster processing
  VideoMain.copyTo(VideoMain_U);
  // convert to float for more accuracy
  VideoMain_U.convertTo(VideoMain_FU, CV_32FC3);

  // filter choices in-series allowable   ***  NOMINAL 3 & 5 on  ***
  if (Ones2x2_A)
    blur(VideoMain_FU, VideoMain_FU, Size(2, 2));
  if (Ones3x3_A)
    blur(VideoMain_FU, VideoMain_FU, Size(3, 3));
  if (Ones4x4_A)
    blur(VideoMain_FU, VideoMain_FU, Size(4, 4));
  if (Ones5x5_A)
    blur(VideoMain_FU, VideoMain_FU, Size(5, 5));
  if (Ones6x6_A)
    blur(VideoMain_FU, VideoMain_FU, Size(6, 6));
  if (Ones7x7_A)
    blur(VideoMain_FU, VideoMain_FU, Size(7, 7));



  // gain and black level
  VideoMain_FU.convertTo(VideoMain_FU, -1, Gain, Black_Level);

  // post gain gamma correction
  RGB_Gamma_Correction(VideoMain_FU, 0.5 );
  // RGB_Gamma_Correction(VideoMain_FU, 0.0 );


  // adjust the color level and gamma!
  // complex because many other ways of doing it had memory leaks or were really slow or both!!
  // for example COLOR_YUV2BGR was really slow and bad memory leak

  // convert to YUV
  cvtColor(VideoMain_FU, VideoTemp_FU3, cv::COLOR_BGR2YUV);
  // extract Luminance 1 channel
  extractChannel(VideoTemp_FU3, Y_FU1, 0);
  // covert Luminance 1 channel to 3 channels
  cvtColor(Y_FU1, VideoTemp_FU3, cv::COLOR_GRAY2BGR);
  // make B-Y  G-Y  R-Y
  subtract(VideoMain_FU, VideoTemp_FU3, Color_Difference_FU3);
  // chroma_gain*((B-Y,  G-Y,  R-Y)
  multiply(Color_Difference_FU3, Color_Gain, Color_Difference_FU3);
  // chroma_gain*((B-Y,  G-Y,  R-Y)  +  B,G,R  to get back to BGR with color level changed
  add(VideoTemp_FU3, Color_Difference_FU3, VideoProc_FU);

  


  // convert back to 8 bits unsigned integer  for the display
  VideoProc_FU.convertTo(VideoMain_U, CV_8UC3);
  // make a copy for the display in 8 bit Mat vs UMat
  VideoMain_U.copyTo(VideoDisplay);
}
