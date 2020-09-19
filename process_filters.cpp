



#include "opencv2/opencv.hpp"
// #include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"

#include "process_filters.h"

using namespace std;
using namespace cv;






// note this is slower than having the code inline by .5 ms  ?????
// somehow the automatic in / out of blur is quicker than cloning 1st
void Filter_Ones(cv::Mat in)
{
    blur(in, in, Size(3,3));
    blur(in, in, Size(5,5));
    blur(in, in, Size(7,7));
}


void Filter_HxV(cv::Mat src)
{
    Mat mkernel_H = (Mat_<double>(1,15) << 1, 2, 3, 4, 5 ,6, 7, 8, 7, 6, 5, 4, 3, 2, 1 )/64;  // works
    Mat mkernel_V = (Mat_<double>(15,1) << 1, 2, 3, 4, 5 ,6, 7, 8, 7, 6, 5, 4, 3, 2, 1 )/64;

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}


void Filter_HxV_2(cv::Mat src)
{
    Mat mkernel_H = (Mat_<double>(1,13) << .01, .032, .06, .087, .113, .13, .136, .13, .113, .087, .06, .032, .01) ;
    Mat mkernel_V = (Mat_<double>(13,1) << .01, .032, .06, .087, .113, .13, .136, .13, .113, .087, .06, .032, .01) ;

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}



void Filter_HxV_3(cv::Mat src)  //   Similiar to the 3 in series ones filter with a 2p2 in series but this is much slower ???
{
    Mat mkernel_H = (Mat_<double>(1,17) << 10, 32, 80, 151, 243, 336, 422, 477, 498, 477, 422, 336, 243, 151, 80, 32, 10)/4000 ;
    Mat mkernel_V = (Mat_<double>(17,1) << 10, 32, 80, 151, 243, 336, 422, 477, 498, 477, 422, 336, 243, 151, 80, 32, 10)/4000 ;

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}

void Filter_HxV_4(cv::Mat src)  //   Similiar to the 3 in series ones filter with a 2p2 in series but this is much slower ???
{
    Mat mkernel_H = (Mat_<double>(1,13) << 1, 3, 6, 9, 12, 14, 15, 14, 12, 9, 6, 3, 1)/105 ;
    Mat mkernel_V = (Mat_<double>(13,1) << 1, 3, 6, 9, 12, 14, 15, 14, 12, 9, 6, 3, 1)/105 ;;

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}




// float data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 10 };
// cv::Mat your_matrix = cv::Mat(1, 10, CV_32F, data








void Filter_HxV_1p1(cv::Mat src)
{
    Mat mkernel_H = (Mat_<double>(1,3) << .25, .5, .25) ;
    Mat mkernel_V = (Mat_<double>(3,1) << .25, .5, .25) ;

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}




void Filter_HxV_2p2(cv::Mat src)
{
    Mat mkernel_H = (Mat_<double>(1,5) << .25, 0, .5, 0, .25) ;
    Mat mkernel_V = (Mat_<double>(5,1) << .25, 0, .5, 0, .25) ;

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}


void Filter_HxV_3p3(cv::Mat src)
{
    Mat mkernel_H = (Mat_<double>(1,7) << .25, 0,  0, .5, 0,  0, .25) ;
    Mat mkernel_V = (Mat_<double>(7,1) << .25, 0,  0, .5, 0,  0, .25) ;

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}


void Filter_HxV_4p4(cv::Mat src)
{
    Mat mkernel_H = (Mat_<double>(1,9) << .25, 0, 0, 0, .5, 0, 0, 0, .25) ;
    Mat mkernel_V = (Mat_<double>(9,1) << .25, 0, 0, 0, .5, 0, 0, 0, .25) ;

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}


void Filter_Gaussian_HxV(cv::Mat src)
{
    GaussianBlur( src, src, Size( 15, 1 ), 0, 0 );  // Horizontal Filter
    GaussianBlur( src, src, Size( 1, 15 ), 0, 0 );  // Vertical Filter
}


void Filter_Combo(cv::Mat in, bool OnBlur, bool On2p2, bool On3p3, bool On4p4)
{
    if(OnBlur)
    {
        blur(in, in, Size(3,3));
        blur(in, in, Size(5,5));
        blur(in, in, Size(7,7));
    }
    if(On2p2)Filter_HxV_2p2(in);
    if(On3p3)Filter_HxV_3p3(in);
    if(On4p4)Filter_HxV_4p4(in);
}


void Filter_3Taps(cv::Mat in, bool On1p1, bool On2p2, bool On3p3, bool On4p4, bool On3Taps)
{
    if(On3Taps)
    {
        if(On1p1)Filter_HxV_1p1(in);
        if(On2p2)Filter_HxV_2p2(in);
        if(On3p3)Filter_HxV_3p3(in);
        if(On4p4)Filter_HxV_4p4(in);
    }

}

void Filter_Blurs(cv::Mat in, bool On2x2, bool On3x3, bool On4x4, bool On5x5, bool On6x6, bool On7x7, bool OnOnes)
{
    if(OnOnes)
    {
        if(On2x2)blur(in, in, Size(2,2));
        if(On3x3)blur(in, in, Size(3,3));
        if(On4x4)blur(in, in, Size(4,4));
        if(On5x5)blur(in, in, Size(5,5));
        if(On6x6)blur(in, in, Size(6,6));
        if(On7x7)blur(in, in, Size(7,7));
    }
}







void Filter_HxV_1p1_U(cv::UMat_Type src)
{
    float coefs[3] = {.25, .5, .25 };
    cv::Mat mkernel_H = cv::Mat(1, 3, CV_32F, coefs);
    cv::Mat mkernel_V = cv::Mat(3, 1, CV_32F, coefs);
    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}


void Filter_HxV_2p2_U(cv::UMat_Type src)
{
    float coefs[5] = {.25,   0, .5,  0, .25 };
    cv::Mat mkernel_H = cv::Mat(1, 5, CV_32F, coefs);
    cv::Mat mkernel_V = cv::Mat(5, 1, CV_32F, coefs);

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}


void Filter_HxV_3p3_U(cv::UMat_Type src)
{
    float coefs[7] = {.25, 0,  0, .5, 0,  0, .25 };
    cv::Mat mkernel_H = cv::Mat(1, 3, CV_32F, coefs);
    cv::Mat mkernel_V = cv::Mat(3, 1, CV_32F, coefs);

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}


void Filter_HxV_4p4_U(cv::UMat_Type src)
{
    float coefs[9] = {.25, 0,  0, 0, .5, 0, 0, 0, .25 };
    cv::Mat mkernel_H = cv::Mat(1, 9, CV_32F, coefs);
    cv::Mat mkernel_V = cv::Mat(9, 1, CV_32F, coefs);

    filter2D(src, src , -1, mkernel_H, Point(-1,-1), 0, BORDER_DEFAULT  );  // Horizontal Filter
    filter2D(src, src , -1, mkernel_V, Point(-1,-1), 0, BORDER_DEFAULT  );    // Vertical Filter
}



void Filter_3Taps_U(cv::UMat_Type in, bool On1p1, bool On2p2, bool On3p3, bool On4p4, bool On3Taps)
{
    if(On3Taps)
    {
        if(On1p1)Filter_HxV_1p1_U(in);
        if(On2p2)Filter_HxV_2p2_U(in);
        if(On3p3)Filter_HxV_3p3_U(in);
        if(On4p4)Filter_HxV_4p4_U(in);
    }
}


void Filter_Blurs_U(cv::UMat_Type in, bool On2x2, bool On3x3, bool On4x4, bool On5x5, bool On6x6, bool On7x7, bool OnOnes)
{
    if(OnOnes)
    {
        if(On2x2)blur(in, in, Size(2,2));
        if(On3x3)blur(in, in, Size(3,3));
        if(On4x4)blur(in, in, Size(4,4));
        if(On5x5)blur(in, in, Size(5,5));
        if(On6x6)blur(in, in, Size(6,6));
        if(On7x7)blur(in, in, Size(7,7));
    }
}


