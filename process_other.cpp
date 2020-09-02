
#include "opencv2/opencv.hpp"

#include <iostream>

#include "defines_Mission_Plaza.h"
#include "measure2.h"
#include "process_other.h"

using namespace std;
using namespace cv;

// this could be faster maybe with an output Mat instead of modifying the input Mat
// shift image HORIZONTALLY with wrap around  Mat
void Shift_Image_Horizontal(cv::Mat &Vid_In, int H_Location)
{
    // note by incrementing the location the movie can shift around in real time.
    // need to figure out how to do this.  Think I need an image class or structure

    int Location_Wrap = H_Location % IMAGE_COLS;

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

// this could be faster maybe with an output Mat instead of modifying the input Mat
// shift image HORIZONTALLY with wrap around  UMat
void Shift_Image_Horizontal_U(cv::UMat &Vid_In, int H_Location)
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

    Location_Wrap = H_Location % IMAGE_COLS;
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

// this could be faster maybe with an output Mat instead of modifying the input Mat
// shift image HORIZONTALLY and VERTICALLY with wrap around  ASSUMES MOVING DOWN ONLY  UMat
void Shift_Image_Horizontal_Vertical_U(cv::UMat &Vid_In, int H_Location, int V_Location)
{
    // made everything static maybe helps memory leaks ?

    static int Location_Wrap;

    static int Fill_Height;

    static int ROI_Width_1;
    static int ROI_Height;
    static int ROI_Width_2;

    static UMat ROI_Before_1;
    static UMat ROI_Before_2;

    static UMat ROI_After_1;
    static UMat ROI_After_2;

    Location_Wrap = H_Location % IMAGE_COLS;
    // cout << endl << "Locatin_Wrap " << Location_Wrap << endl;

    Fill_Height = IMAGE_ROWS - V_Location;

    ROI_Width_1 = IMAGE_COLS - Location_Wrap;
    ROI_Height = Fill_Height;
    ROI_Width_2 = IMAGE_COLS - ROI_Width_1;

    Rect Rect_Before_1(0, 0, ROI_Width_1, Fill_Height);
    Rect Rect_Before_2(ROI_Width_1, 0, ROI_Width_2, Fill_Height);

    Rect Rect_After_1(ROI_Width_2, V_Location, ROI_Width_1, Fill_Height);
    Rect Rect_After_2(0, V_Location, ROI_Width_2, Fill_Height);

    ROI_Before_1 = Vid_In(Rect_Before_1).clone();
    ROI_Before_2 = Vid_In(Rect_Before_2).clone();

    ROI_After_1 = Vid_In(Rect_After_2); // Get the header to the destination position
    ROI_After_2 = Vid_In(Rect_After_1); // Get the header to the destination position

    ROI_Before_1.copyTo(ROI_After_2);
    ROI_Before_2.copyTo(ROI_After_1);
}

//  this could be faster maybe with an output Mat instead of modifying the input Mat
//  shift image HORIZONTALLY and VERTICALLY with wrap around  ASSUMES MOVING DOWN ONLY  UMat
//  Fills above  the shift with the Bkgnd
void Shift_Image_Horizontal_Vertical_U2(cv::UMat &Vid_In, cv::UMat &Vid_Out, int H_Location, int V_Location, UMat &Bkgnd)
{
    // made everything static maybe helps memory leaks ?

    static int Location_Wrap;

    static int Image_Fill_Height;

    static int Bkgnd_Fill_Height;

    static int ROI_Width_1;
    static int ROI_Height;
    static int ROI_Width_2;

    static UMat ROI_Before_1;
    static UMat ROI_Before_2;

    static UMat ROI_After_1;
    static UMat ROI_After_2;

    static UMat ROI_Bkgnd_Fill_Before;
    static UMat ROI_Bkgnd_Fill_After;

    Location_Wrap = H_Location % IMAGE_COLS;
    // cout << endl << "Locatin_Wrap " << Location_Wrap << endl;

    Image_Fill_Height = IMAGE_ROWS - V_Location;

    Bkgnd_Fill_Height = V_Location;

    ROI_Width_1 = IMAGE_COLS - Location_Wrap;
    ROI_Height = Image_Fill_Height;
    ROI_Width_2 = IMAGE_COLS - ROI_Width_1;

    Rect Rect_Before_1(0, 0, ROI_Width_1, Image_Fill_Height);
    Rect Rect_Before_2(ROI_Width_1, 0, ROI_Width_2, Image_Fill_Height);

    Rect Rect_After_1(ROI_Width_2, V_Location, ROI_Width_1, Image_Fill_Height);
    Rect Rect_After_2(0, V_Location, ROI_Width_2, Image_Fill_Height);

    ROI_Before_1 = Vid_In(Rect_Before_1).clone();
    ROI_Before_2 = Vid_In(Rect_Before_2).clone();

    ROI_After_1 = Vid_In(Rect_After_2); // Get the header to the destination position
    ROI_After_2 = Vid_In(Rect_After_1); // Get the header to the destination position

    ROI_Before_1.copyTo(ROI_After_2);
    ROI_Before_2.copyTo(ROI_After_1);

    Vid_Out = Vid_In.clone();

    Rect Rect_Fill(0, 0, IMAGE_COLS, Bkgnd_Fill_Height);
    ROI_Bkgnd_Fill_Before = Bkgnd(Rect_Fill).clone();
    ROI_Bkgnd_Fill_After = Vid_Out(Rect_Fill); // Get the header to the destination position
    ROI_Bkgnd_Fill_Before.copyTo(ROI_Bkgnd_Fill_After);
}

void Shift_Image_Horizontal_Vertical_U3(cv::UMat &Vid_In, cv::UMat &Vid_Out, int H_Location, int V_Location, UMat &Bkgnd)
{
    // made everything static maybe helps memory leaks ?

    static int Location_Wrap;

    static int Image_Fill_Height;

    static int Bkgnd_Fill_Height;

    static int ROI_Width_1;
    static int ROI_Height;
    static int ROI_Width_2;

    static UMat ROI_Before_1;
    static UMat ROI_Before_2;

    static UMat ROI_After_1;
    static UMat ROI_After_2;

    static UMat ROI_Bkgnd_Fill_Before;
    static UMat ROI_Bkgnd_Fill_After;

    Rect Rect_Before_1, Rect_Before_2, Rect_After_1, Rect_After_2;

    Rect Rect_Fill;

    Location_Wrap = H_Location % IMAGE_COLS;
    // cout << endl << "Locatin_Wrap " << Location_Wrap << endl;

    ROI_Width_1 = IMAGE_COLS - Location_Wrap;
    ROI_Width_2 = IMAGE_COLS - ROI_Width_1;

    if (V_Location >= 0)
    {
        Image_Fill_Height = IMAGE_ROWS - V_Location;
        ROI_Height = Image_Fill_Height;

        Rect_Before_1 = Rect(0, 0, ROI_Width_1, Image_Fill_Height);
        Rect_Before_2 = Rect(ROI_Width_1, 0, ROI_Width_2, Image_Fill_Height);

        Rect_After_1 = Rect(ROI_Width_2, V_Location, ROI_Width_1, Image_Fill_Height);
        Rect_After_2 = Rect(0, V_Location, ROI_Width_2, Image_Fill_Height);
    }
    else // V_Location < 0
    {
        Image_Fill_Height = IMAGE_ROWS + V_Location;
        ROI_Height = Image_Fill_Height;

         Rect_Before_1 = Rect(0, -V_Location, ROI_Width_1, Image_Fill_Height);
         Rect_Before_2 = Rect(ROI_Width_1, -V_Location, ROI_Width_2, Image_Fill_Height);

         Rect_After_1 = Rect(ROI_Width_2, 0, ROI_Width_1, Image_Fill_Height);
         Rect_After_2 = Rect(0, 0, ROI_Width_2, Image_Fill_Height);
    }

    ROI_Before_1 = Vid_In(Rect_Before_1).clone();
    ROI_Before_2 = Vid_In(Rect_Before_2).clone();

    ROI_After_1 = Vid_In(Rect_After_2); // Get the header to the destination position
    ROI_After_2 = Vid_In(Rect_After_1); // Get the header to the destination position

    ROI_Before_1.copyTo(ROI_After_2);
    ROI_Before_2.copyTo(ROI_After_1);

    Vid_Out = Vid_In.clone();

    if (V_Location >= 0)
    {
        Bkgnd_Fill_Height = V_Location;
        Rect_Fill = Rect(0, 0, IMAGE_COLS, Bkgnd_Fill_Height);
        ROI_Bkgnd_Fill_Before = Bkgnd(Rect_Fill).clone();
        ROI_Bkgnd_Fill_After = Vid_Out(Rect_Fill); // Get the header to the destination position
        ROI_Bkgnd_Fill_Before.copyTo(ROI_Bkgnd_Fill_After);
    }
    else
    {
        Bkgnd_Fill_Height = -V_Location;
        Rect_Fill = Rect(0, IMAGE_ROWS + V_Location, IMAGE_COLS, Bkgnd_Fill_Height);
        ROI_Bkgnd_Fill_Before = Bkgnd(Rect_Fill).clone();
        ROI_Bkgnd_Fill_After = Vid_Out(Rect_Fill); // Get the header to the destination position
        ROI_Bkgnd_Fill_Before.copyTo(ROI_Bkgnd_Fill_After);
    }

}

// this could be faster maybe with an output Mat instead of modifying the input Mat
//  shift image VERTICALLY  ASSUMES MOVING DOWN ONLY  UMat
void Shift_Image_Vertical_U(cv::UMat &Vid_In, int V_Location)
{
    // made everything static maybe helps memory leaks ?
    static int ROI_Width;
    static int ROI_Height;
    static UMat ROI_Before;
    static UMat ROI_After;

    ROI_Width = IMAGE_COLS;
    ROI_Height = IMAGE_ROWS - V_Location;

    Rect Rect_V_In(0, 0, ROI_Width, ROI_Height);
    Rect Rect_V_Shift(0, V_Location, ROI_Width, ROI_Height);

    ROI_Before = Vid_In(Rect_V_In).clone();
    ROI_After = Vid_In(Rect_V_Shift); // Get the header to the destination position
    ROI_Before.copyTo(ROI_After);
}

// this could be faster maybe with an output Mat instead of modifying the input Mat
// this one assumes that the shift on main is always down
//  shift image VERTICALLY  ASSUMES MOVING DOWN ONLY  UMat
// FILLS above the shift with Bkgnd Mat
void Shift_Image_Vertical_U(cv::UMat &Vid_In, int V_Location, cv::UMat &Bkgnd)
{
    // made everything static maybe helps memory leaks ?
    static int ROI_Width;
    static int ROI_Height_Main;
    static UMat ROI_Main_Before;
    static UMat ROI_Main_After;

    static int ROI_Height_Fill;
    static UMat ROI_Fill_Before;
    static UMat ROI_Fill_After;

    ROI_Width = IMAGE_COLS;
    ROI_Height_Main = IMAGE_ROWS - V_Location;

    ROI_Height_Fill = V_Location;

    Rect Rect_V_Main_In(0, 0, ROI_Width, ROI_Height_Main);
    Rect Rect_V_Main_Shift(0, V_Location, ROI_Width, ROI_Height_Main);
    ROI_Main_Before = Vid_In(Rect_V_Main_In).clone();
    ROI_Main_After = Vid_In(Rect_V_Main_Shift); // Get the header to the destination position
    ROI_Main_Before.copyTo(ROI_Main_After);

    Rect Rect_Fill_In(0, 0, ROI_Width, ROI_Height_Fill);
    ROI_Fill_Before = Bkgnd(Rect_Fill_In).clone();
    ROI_Fill_After = Vid_In(Rect_Fill_In); // Get the header to the destination position
    ROI_Fill_Before.copyTo(ROI_Fill_After);
}

// this could be faster maybe with an output Mat instead of modifying the input Mat
// this one assumes that the shift on main is always down
// shift image VERTICALLY  ASSUMES MOVING DOWN ONLY  UMat
//  has an output UMat as opposed to modifying the input !!!!!!!!!!!!!
// FILLS above the shift with Bkgnd Mat
void Shift_Image_Vertical_U2(cv::UMat &Vid_In, UMat &Vid_Out, int V_Location, cv::UMat &Bkgnd)
{
    // made everything static maybe helps memory leaks ?
    static int ROI_Width;
    static int ROI_Height_Main;
    static UMat ROI_Main_Before;
    static UMat ROI_Main_After;

    // Vid_Out = Bkgnd.clone();

    Bkgnd.copyTo(Vid_Out);

    ROI_Width = IMAGE_COLS;
    ROI_Height_Main = IMAGE_ROWS - V_Location;

    Rect Rect_V_Main_In(0, 0, ROI_Width, ROI_Height_Main);
    Rect Rect_V_Main_Shift(0, V_Location, ROI_Width, ROI_Height_Main);

    ROI_Main_Before = Vid_In(Rect_V_Main_In).clone();
    ROI_Main_After = Vid_Out(Rect_V_Main_Shift); // Get the header to the destination position
    ROI_Main_Before.copyTo(ROI_Main_After);
}
