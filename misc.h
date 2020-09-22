
#include <string>


float Light_Sensor_Process(float Light_Sensor_Raw, float Light_Sensor_Gamma_Level, float &Light_Sensor_Fil);

// store sunrise and sunsets for certain dates in seconds
int Convert_Sun_File(std::string  Sun_Info[][12], int Sun_Dates_Times[12][5][3]);


float Day_Night_Final_Gain(int Sun_Dates_Times[12][5][3], int64_t &Current_Sunrise, int64_t &Current_Sunset );
