#ifndef DEFINES_H
#define DEFINES_H
#pragma once




// should these be defines or constants ????

#define Sculpture_Size_Pixels       11136       //  4 * 16 bit words per pixel

#define Sculpture_Size_RGBW             44544   //  16 bit   4 * 11136
#define Sculpture_Size_RGBW_Bytes       89088   //  16 bit   8 * 11136

#define Buffer_W_Gaps_Size_RGBW         52464   //  16 bit   4 * 13116
#define Buffer_W_Gaps_Size_RGBW_Bytes   104928  //  16 bit   8 * 13116

#define Buffer_W_Gaps_Size_RGBW_Extra         55000   //  16 bit   4 * 13116
#define Buffer_W_Gaps_Size_RGBW_Bytes_Extra   110000  //  16 bit   8 * 13116


#define Sculpture_Size_RGB              33408   //  16 bit   3 * 11136
#define Sculpture_Size_RGB_Bytes        66816   //  16 bit   6 * 11136

#define Buffer_W_Gaps_Size_RGB          39348   //  16 bit   3 * 13116
#define Buffer_W_Gaps_Size_RGB_Bytes    78696   //  16 bit   3 * 13116


const int IMAGE_ROWS = 280;
const int IMAGE_COLS = 1024;



// 1K multiples
// 12288 "pixels"   49152 words  98304 bytes


#define TxBuffer_Size_Words 49152   //  FTDI needs multiple of 1024
#define TxBuffer_Size_Bytes 98304   //  FTDI needs multiple of 1024
#define	RxBuffer_Size 1024


#define Max_Num_Of_Image_Files      100
#define Max_File_Name_Length        25
#define Max_Num_Of_Image_Parameters 50

// const int NUM_OF_ENCLOSURES = 32;
// const int NUM_OF_ENCLOSURE_PARAMETERS = 17;
const int MAX_NUM_OF_PANELS = 6;







#define Gain_X 0
#define Black_Level_X 1
#define Chroma_X 2
#define Red_Val 3
#define Green_Val 4
#define Blue_Val 5
#define Duration 6


#define Red_Val_62      7
#define Green_Val_62    8
#define Blue_Val_62     9
#define RGB_Gain_62     10


#define Red_Val_63   11
#define Green_Val_63 12
#define Blue_Val_63  13
#define RGB_Gain_63  14

#define Red_Val_64   15
#define Green_Val_64 16
#define Blue_Val_64  17
#define RGB_Gain_64  18

#define Red_Val_65   19
#define Green_Val_65 21
#define Blue_Val_65  21
#define RGB_Gain_65  22

#define Red_Val_Filament    23
#define Green_Vallament     24
#define Blue_Vallament      25
#define RGB_Gain_Filament   26

#define Red_Val_Constellation   27
#define Green_Val_Constellation 28
#define Blue_Val_Constellation  29
#define White_Val_Constellation  30
#define RGBW_Gain_Constellation  31



// Dissolve defines

#define     ToA          1.0f
#define     ToB         -1.0f


#define     Image_Fading  0
#define     Image_A  1
#define     Image_B  2

#define RGB 3
#define RGBW 4

#define Words_Per_Pixel 4

#define Mapped_Enclosure_Address 1
#define First_Pixel_Location 2
#define Panel1_Offset 3
#define Green_DAC 9
#define Panel1_LR 10



#endif /* DEFINES_H */
