
#include "misc.h"
#include <string>
#include "defines_Mission_Plaza.h"

// float Light_Sensor_Process(float Light_Sensor_Raw, float Light_Sensor_Gamma_Level, float &Light_Sensor_Fil)
// {

//    // static float Light_Sensor_Fil;
//     float Light_Sensor_Sub, Light_Sensor_Scaled, Light_Sensor_Scaled_Full_Gamma, Light_Sensor_Curved, Light_Sensor_Final_Gain;


//     /// filter the light sensor info
//     Light_Sensor_Fil = ( Light_Sensor_TC * Light_Sensor_Raw )  +  ( (1-Light_Sensor_TC) * Light_Sensor_Fil  ) ;

//     /// subject the min threshold
//     Light_Sensor_Sub = Light_Sensor_Fil - Light_Sensor_Lower_Limit ;
//     if(Light_Sensor_Sub < 0)Light_Sensor_Sub = 0;

//     /// scale from 0 - 1
//     Light_Sensor_Scaled = Light_Sensor_Sub /  (Light_Sensor_Upper_Limit - Light_Sensor_Lower_Limit) ;
//     if(Light_Sensor_Scaled > 1.0)Light_Sensor_Scaled = 1.0 ;

//     /// non linear gamma crv approximation
//     Light_Sensor_Scaled_Full_Gamma = pow( Light_Sensor_Scaled,  Light_Sensor_Gamma_Power) ;

//     /// gamma adjust
//      Light_Sensor_Curved = (Light_Sensor_Gamma_Level * Light_Sensor_Scaled_Full_Gamma) + ( (1.0 - Light_Sensor_Gamma_Level) * Light_Sensor_Scaled)  ;

//     Light_Sensor_Final_Gain =   (Light_Sensor_Curved * (Light_Sensor_Gain_Upper_Limit - Light_Sensor_Gain_Lower_Limit)  ) + Light_Sensor_Gain_Lower_Limit ;

//     return Light_Sensor_Final_Gain;
// }



// store sunrise and sunsets for certain dates in seconds
int Convert_Sun_File(std::string  Sun_Info[][12], int Sun_Dates_Times[12][5][3])
{
    std::string str1;
    int    old_month, month, day, hour_rise, minute_rise, hour_set, minute_set;

    int     Day_Increment=0;

    old_month = 0;
    month = 0;

    std::string::size_type sz;

    for(int date=0; date <53; date++)
    {

        str1 =  Sun_Info[date][0].substr(0, 2) ;
        month = std::stoi (str1,&sz);

        if(old_month !=  month)Day_Increment = 0;
        old_month = month;

        str1 =  Sun_Info[date][0].substr(3, 4) ;
        day = std::stoi (str1,&sz);

        str1 =  Sun_Info[date][1].substr(0, 2) ;
        hour_rise = std::stoi (str1,&sz);

        str1 =  Sun_Info[date][1].substr(3, 4) ;
        minute_rise = std::stoi (str1,&sz);

        str1 =  Sun_Info[date][2].substr(0, 2) ;
        hour_set = std::stoi (str1,&sz);

        str1 =  Sun_Info[date][2].substr(3, 4) ;
        minute_set = std::stoi (str1,&sz);


        Sun_Dates_Times[month-1][Day_Increment][0] = day;
        Sun_Dates_Times[month-1][Day_Increment][1] = hour_rise * 3600 + minute_rise * 60;
        Sun_Dates_Times[month-1][Day_Increment][2] = hour_set  * 3600 + minute_set  * 60;

  //      cout << month  <<  "  "  <<  Day_Increment << "  "  << day  << "  " << hour_rise << "  " << minute_rise <<  "  " << hour_set <<  "  "
  //              << minute_set  << "  "  <<  Sun_Dates_Times[month-1][day][0]  << "  "  <<  Sun_Dates_Times[month-1][day][1]  << endl ;

        Day_Increment++;
    }
    return 0;
}


///struct tm {
///   int tm_sec;   // seconds of minutes from 0 to 61
///   int tm_min;   // minutes of hour from 0 to 59
///   int tm_hour;  // hours of day from 0 to 24
///   int tm_mday;  // day of month from 1 to 31
///   int tm_mon;   // month of year from 0 to 11
///   int tm_year;  // year since 1900
///   int tm_wday;  // days since sunday
///   int tm_yday;  // days since January 1st
///   int tm_isdst; // hours of daylight savings time
///}

float Day_Night_Final_Gain(int Sun_Dates_Times[12][5][3], int64_t &Current_Sunrise, int64_t &Current_Sunset )
{
    time_t time_time;
    struct tm * time_X;
    int64_t Current_Month, Current_Month_Mod;
    int64_t Current_Date;
    int64_t Daylight_ST;
    int64_t Current_Hour;
    int64_t Current_Minute;
    int64_t Current_Second;
    int64_t Time_Current_Total ;

    int64_t Sunrise, Sunset, Sunrise_Time_Diff, Sunset_Time_Diff ;

    int16_t index, index_mod;

    float Sun_Gain;

    time (&time_time);
    time_X = localtime(&time_time);
    Current_Month = time_X->tm_mon;
    Current_Date = time_X->tm_mday;
    Daylight_ST = time_X->tm_isdst;
    Current_Hour = time_X->tm_hour;
    Current_Minute = time_X->tm_min;
    Current_Second = time_X->tm_sec;

    Time_Current_Total = Current_Hour * 3600 + Current_Minute * 60 + Current_Second;


    // index -1 means last month  5 means next month
    if(  (Current_Date - Sun_Dates_Times[Current_Month][0][0] ) < -3) index = -1 ;
    else if( ( (Current_Date - Sun_Dates_Times[Current_Month][0][0] ) >= -3) && ( (Current_Date - Sun_Dates_Times[Current_Month][0][0] ) <= 3) )index = 0 ;
    else if( ( (Current_Date - Sun_Dates_Times[Current_Month][1][0] ) >= -3) && ( (Current_Date - Sun_Dates_Times[Current_Month][1][0] ) <= 3) )index = 1 ;
    else if( ( (Current_Date - Sun_Dates_Times[Current_Month][2][0] ) >= -3) && ( (Current_Date - Sun_Dates_Times[Current_Month][2][0] ) <= 3) )index = 2 ;
    else if( ( (Current_Date - Sun_Dates_Times[Current_Month][3][0] ) >= -3) && ( (Current_Date - Sun_Dates_Times[Current_Month][3][0] ) <= 3) )index = 3 ;
    else if( ( (Current_Date - Sun_Dates_Times[Current_Month][4][0] ) >= -3) && ( (Current_Date - Sun_Dates_Times[Current_Month][4][0] ) <= 3) )index = 4 ;
    else index = 5 ;

    Current_Month_Mod = Current_Month ;
    if(index == -1)  Current_Month_Mod = Current_Month-1 ;
    else if(index ==  5)  Current_Month_Mod = Current_Month+1 ;

    index_mod = index ;
    if(index == -1)  index_mod =   ( Sun_Dates_Times[Current_Month_Mod][4][1] == 0)  ? 3 : 4 ;
    else if(index ==  5)  index_mod =   0;

    Daylight_ST = Daylight_ST * 3600 ;  // add an hour during daylight savings time
    Sunrise = Sun_Dates_Times[Current_Month_Mod][index_mod][1]  + Daylight_ST ;
    Sunset = Sun_Dates_Times[Current_Month_Mod][index_mod][2]   + Daylight_ST ;

    // +- 20 minutes of range
    Sunrise_Time_Diff = Time_Current_Total - Sunrise ;   // positive daytime
    Sunset_Time_Diff  = Sunset - Time_Current_Total;     // positive daytime

    if( (Sunrise_Time_Diff >=  Sun_Time_Window_Seconds ) &&    (Sunset_Time_Diff >= Sun_Time_Window_Seconds) )Sun_Gain = Sun_Gain_Upper_Limit;
    else if( (Sunrise_Time_Diff <=  -Sun_Time_Window_Seconds )  ||    (Sunset_Time_Diff <= -Sun_Time_Window_Seconds) )Sun_Gain = Sun_Gain_Lower_Limit;

    else if(abs(Sunrise_Time_Diff) <= Sun_Time_Window_Seconds )Sun_Gain = ((float)Sunrise_Time_Diff + (float)Sun_Time_Window_Seconds) / (2*(float)Sun_Time_Window_Seconds)  ;
    else Sun_Gain  = ((float)Sunset_Time_Diff  + (float)Sun_Time_Window_Seconds) / (2*(float)Sun_Time_Window_Seconds)  ;   // (abs(Sunset_Time_Diff) <= 1200 )S

  //  printf("Time_cur_Total %d  Months %d %d  Date %d DST %d  index %d %d   SS  %d %d ", Time_Current_Total,  Current_Month, Current_Month_Mod, Current_Date, Daylight_ST,  index, index_mod, Sunrise, Sunset );
  //  printf("\nTimeTotal %d  SR %d SS %d  SR_Diff %d   SS_Diff %d  SGain %.2f \n\n", Time_Current_Total, Sunrise, Sunset, Sunrise_Time_Diff, Sunset_Time_Diff, Sun_Gain );
    Current_Sunrise = Sunrise;
    Current_Sunset = Sunset;

    return Sun_Gain ;

}


