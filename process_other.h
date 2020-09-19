#ifndef PROCESS_OTHER_H
#define PROCESS_OTHER_H
#pragma once

#include "defines_Mission_Plaza.h"

using namespace std;
using namespace cv;



void Shift_Image_Horizontal(cv::Mat& Vid_In, int InitH_Locational_Location );
void Shift_Image_Horizontal_U(cv::UMat_Type& Vid_In, int H_Location );

void Shift_Image_Horizontal_Vertical_U(cv::UMat_Type& Vid_In, int H_Location, int V_Location );
void Shift_Image_Horizontal_Vertical_U2(cv::UMat_Type &Vid_In, cv::UMat_Type &Vid_Out, int H_Location, int V_Location, UMat_Type & Bkgnd);
void Shift_Image_Horizontal_Vertical_U3(cv::UMat_Type &Vid_In, cv::UMat_Type &Vid_Out, int H_Location, int V_Location, UMat_Type & Bkgnd);

void Shift_Image_Vertical_U(cv::UMat_Type &Vid_In, int V_Location);

void Shift_Image_Vertical_U(cv::UMat_Type &Vid_In, int V_Location, cv::UMat_Type & Bkgnd);

void Shift_Image_Vertical_U2(cv::UMat_Type &Vid_In, UMat_Type &Vid_Out, int V_Location, cv::UMat_Type & Bkgnd);


#endif /* PROCESS_OTHER_H */
