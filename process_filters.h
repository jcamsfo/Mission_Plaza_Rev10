//  Filtering Declarations

#ifndef PROCESS_FILTERS_H
#define PROCESS_FILTERS_H
#pragma once



void Filter_Ones(cv::Mat in);
void Filter_HxV(cv::Mat src);
void Filter_HxV_2(cv::Mat src);
void Filter_HxV_3(cv::Mat src);
void Filter_HxV_4(cv::Mat src);
void Filter_HxV_1p1(cv::Mat src);
void Filter_HxV_2p2(cv::Mat src);
void Filter_HxV_3p3(cv::Mat src);
void Filter_HxV_4p4(cv::Mat src);
void Filter_Gaussian_HxV(cv::Mat src);
void Filter_Combo(cv::Mat in, bool OnBlur, bool On2p2, bool On3p3, bool On4p4);
void Filter_3Taps(cv::Mat in, bool On1p1, bool On2p2, bool On3p3, bool On4p4, bool On3Taps);
void Filter_Blurs(cv::Mat in, bool On2x2, bool On3x3, bool On4x4, bool On5x5, bool On6x6, bool On7x7, bool OnOnes);

void All_Filters(bool *Reset_All);



// UMat versions
void Filter_HxV_1p1_U(cv::UMat src);
void Filter_HxV_2p2_U(cv::UMat src);
void Filter_HxV_3p3_U(cv::UMat src);
void Filter_HxV_4p4_U(cv::UMat src);
void Filter_3Taps_U(cv::UMat in, bool On1p1, bool On2p2, bool On3p3, bool On4p4, bool On3Taps);
void Filter_Blurs_U(cv::UMat in, bool On2x2, bool On3x3, bool On4x4, bool On5x5, bool On6x6, bool On7x7, bool OnOnes);




#endif /* PROCESS_FILTERS_H */


