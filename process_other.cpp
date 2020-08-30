
#include "opencv2/opencv.hpp"

#include <iostream>

#include "defines_Mission_Plaza.h"
#include "measure2.h"
#include "process_other.h"

using namespace std;
using namespace cv;

void Shift_Image_Horizontal(cv::Mat &Vid_In, int Initial_Location)
{
    // note by incrementing the location the movie can shift around in real time.
    // need to figure out how to do this.  Think I need an image class or structure

    int Location_Wrap = Initial_Location % IMAGE_COLS;

    int ROI_Width_1 = IMAGE_COLS - Location_Wrap;
    int ROI_Height = IMAGE_ROWS;
    int ROI_Width_2 = IMAGE_COLS - ROI_Width_1;

    Rect Rect_Before_1(0, 0, ROI_Width_1, ROI_Height);
    Rect Rect_Before_2(ROI_Width_1, 0, ROI_Width_2, ROI_Height);

    Rect Rect_After_1(ROI_Width_2, 0, ROI_Width_1, ROI_Height);
    Rect Rect_After_2(0, 0, ROI_Width_2, ROI_Height);

    Mat ROI_Before_1 = Vid_In(Rect_Before_1).clone();
    Mat ROI_Before_2 = Vid_In(Rect_Before_2).clone();

    Mat ROI_After_1 = Vid_In(Rect_After_2); // Get the header to the destination position
    Mat ROI_After_2 = Vid_In(Rect_After_1); // Get the header to the destination position

    ROI_Before_1.copyTo(ROI_After_2);
    ROI_Before_2.copyTo(ROI_After_1);
}

void Shift_Image_Horizontal_U(cv::UMat &Vid_In, int Initial_Location)
{
    // made everything static maybe for memory leaks ?

    static int Location_Wrap;

    static int ROI_Width_1;
    static int ROI_Height;
    static int ROI_Width_2;

    static UMat ROI_Before_1;
    static UMat ROI_Before_2;

    static UMat ROI_After_1;
    static UMat ROI_After_2;

    Location_Wrap = Initial_Location % IMAGE_COLS;
    // cout << endl << "Locatin_Wrap " << Location_Wrap << endl;

    ROI_Width_1 = IMAGE_COLS - Location_Wrap;
    ROI_Height = IMAGE_ROWS;
    ROI_Width_2 = IMAGE_COLS - ROI_Width_1;

    Rect Rect_Before_1(0, 0, ROI_Width_1, ROI_Height);
    Rect Rect_Before_2(ROI_Width_1, 0, ROI_Width_2, ROI_Height);

    Rect Rect_After_1(ROI_Width_2, 0, ROI_Width_1, ROI_Height);
    Rect Rect_After_2(0, 0, ROI_Width_2, ROI_Height);

    ROI_Before_1 = Vid_In(Rect_Before_1).clone();
    ROI_Before_2 = Vid_In(Rect_Before_2).clone();

    ROI_After_1 = Vid_In(Rect_After_2); // Get the header to the destination position
    ROI_After_2 = Vid_In(Rect_After_1); // Get the header to the destination position

    ROI_Before_1.copyTo(ROI_After_2);
    ROI_Before_2.copyTo(ROI_After_1);
}
