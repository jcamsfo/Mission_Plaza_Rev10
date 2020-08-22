


#include "measure2.h"


using namespace std::chrono;
using namespace std;





// void Read_1D(vector<int> Vec_1Dim, string file_name)
// {
// 	std::ifstream f(file_name);
// 	float Val = 0;
// 	while (f >> Val)
// 		Vec_1Dim.push_back(Val);
// 	f.close();
// }



Prog_Durations::Prog_Durations(void)
{
    update_length = 30;
    accum_count = 0;
    time_delay_accum_temp = 0;
    time_delay_avg = 0;
    Trigger = false;
}

Prog_Durations::Prog_Durations(int16_t Update_Len)
{
    update_length = Update_Len;
    accum_count = 0;
    time_delay_accum_temp = 0;
    time_delay_avg = 0;
    Trigger = false;
}

void Prog_Durations::Start_Delay_Timer(void)
{
    Time_Stamp = high_resolution_clock::now();
}

double Prog_Durations::End_Delay_Timer(void)
{
    Time_Stamp_Current = high_resolution_clock::now();
    time_delay = (duration_cast<microseconds>(Time_Stamp_Current - Time_Stamp).count()) / 1000.;
    loop_Count++;
    if (loop_Count >= 10) // get past the program startup stuff
    {
        accum_count++;
        if (accum_count >= update_length)
        {
            accum_count = 0;
            time_delay_avg = time_delay_accum_temp / (float)update_length;
            time_delay_accum_temp = time_delay;
            time_delay_max = time_delay;
            time_delay_min = time_delay;
            Trigger = true;
        }
        else
        {
            Trigger = false;
            time_delay_accum_temp += time_delay;

            //    time_delay_avg = time_delay_accum / ((float) (loop_Count - 99)  );
            if (time_delay > time_delay_max)
                time_delay_max = time_delay;
            if (time_delay < time_delay_min)
                time_delay_min = time_delay;
        }

        return time_delay;
    }
    else
    {
        time_delay = 0;
        time_delay_avg = 0;
        time_delay_max = 0;
        time_delay_min = 0;
    }
};

void Delay_Msec(float msec)
{
    float del;
    Prog_Durations PD1(30);
    del = 0;
    PD1.Start_Delay_Timer();
    while (del < msec)
    {
        PD1.End_Delay_Timer();
        del = PD1.time_delay;
    }
}
