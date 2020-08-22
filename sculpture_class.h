#ifndef VIDEO_SCULPTURE_CLASS_H
#define VIDEO_SCULPTURE_CLASS_H
#pragma once

#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "player_class.h"
#include "defines.h"
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
    uint16_t *Sampled_Buffer_RGBW;
    float *Sampled_Buffer_RGBW_AF;

    uint16_t *Samples_Mapped_To_Sculpture;


    // Grab the pixels from the image and store them in
    void Save_Samples_From_CSV_Map_To_Buffer_RGBW_Convert_Rev5(void);

    void Map_Subsampled_To_Sculpture(uint16_t *Vid_Sampled, uint16_t *Vid_Mapped, int *Panel_Map_Local, int xx);

    void Map_Subsampled_To_Sculpture(void);

    void Multi_Map_Image_To_Sculpture(void);

    void Add_Visible_Sample_Locations_From_Sample_Points_Map(cv::Mat src) ;

    void Add_Visible_Sample_Locations_From_Sample_Points_Map_Ver2(cv::Mat src) ;

    void Generate_Subsampled_Image_For_Test(uint16_t *Buffer, bool RGBW_or_RGB, vector<vector<int>> X_Sample_Points, int Y_Start, int X_Increment, int X_Start, int Y_Increment);

    void Add_Headers(void);

    void Add_DMX(void);

    Video_Player_With_Processing VP1x;
    Video_Player_With_Processing VP2x;

    bool display_on_X;

    // for display conversion
    UMat VideoSum_FU, VideoSum_U;
    Mat VideoSumDisplay;

    // for mapping to sculpture
    Mat VideoSum_F;
    UMat VideoSum_Small_FU;
    Mat VideoSum_Small_F;
    Mat VideoSum_Small_16;

    Mat Sample_Point_Mat;

    int local_oop;

      Prog_Durations time_test;
};

#endif /* VIDEO_SCULPTURE_CLASS_H */
