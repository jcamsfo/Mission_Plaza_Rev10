
#include "opencv2/opencv.hpp"

#include <iostream>

#include "defines.h"
#include "measure2.h"
#include "process_other.h"


using namespace std;
using namespace cv;


void Shift_Image_Horizontal(cv::Mat& Vid_In, int Initial_Location )
{
    // note by incrementing the location the movie can shift around in real time.
    // need to figure out how to do this.  Think I need an image class or structure

	int ROI_Width_1 = IMAGE_COLS - Initial_Location ;
    int ROI_Height 	= IMAGE_ROWS;
	int ROI_Width_2 = IMAGE_COLS - ROI_Width_1;

	static int Current_Location;

	Rect Rect_Before_1(0, 0, ROI_Width_1, ROI_Height);
	Rect Rect_Before_2(ROI_Width_1, 0, ROI_Width_2, ROI_Height);

	Rect Rect_After_1(ROI_Width_2, 0, ROI_Width_1, ROI_Height);
	Rect Rect_After_2(0, 0, ROI_Width_2, ROI_Height);


    Mat ROI_Before_1 = Vid_In(Rect_Before_1).clone();
    Mat ROI_Before_2 = Vid_In(Rect_Before_2).clone();

    Mat ROI_After_1  = Vid_In(Rect_After_2);            // Get the header to the destination position
    Mat ROI_After_2  = Vid_In(Rect_After_1);            // Get the header to the destination position

    ROI_Before_1.copyTo(ROI_After_2);
    ROI_Before_2.copyTo(ROI_After_1);
}






