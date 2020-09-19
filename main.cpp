

#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <thread>
#include <iomanip>

#include <iostream>
#include <cstdio>
#include <time.h>
#include <ctime>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <functional> // std::plus
#include <string>

#include "defines_Mission_Plaza.h"
#include "player_class.h"
#include "measure2.h"
#include "USB_FTDI.h"
#include "sculpture_class.h"

#include "file_io_2.h"

using namespace std;
using namespace cv;





bool FTDI_On = true;

int main()
{

  char *TxBuffer;
  char *RxBuffer;

  RxBuffer = new char[RxBuffer_Size];
  TxBuffer = new char[Buffer_W_Gaps_Size_RGBW_Bytes_Extra];

  USB_FTDI_Channel FTX(Buffer_W_Gaps_Size_RGBW_Bytes_Extra, RxBuffer_Size);

  Video_Sculpture SC1;

  Prog_Durations Time_Delay[10];
  Prog_Durations Time_Delay_1(30);
  Prog_Durations Time_Delay_2(30);
  Prog_Durations Time_Delay_3(30);
  Prog_Durations Program_Timer(30);
  Prog_Durations Loop_Timer(30);
  Prog_Durations Process_Time(30);

  bool Finished = false;
  bool pauseX = false;
  bool pauseX_old;
  uint16_t last_c;
  int32_t loop = 0;
  float delx;
  uint32_t loopx = 0;
  uint32_t Loop_Cnt = 0;
  uint32_t Bytes_Wrote, Bytes_Read;

  SC1.Read_Maps();

  // for (int ii = 0; ii < Buffer_W_Gaps_Size_RGBW_Bytes_Extra; ii++)
  //   *(TxBuffer + ii) = (unsigned char)ii;

  while (1)
  {
    Process_Time.Start_Delay_Timer();
    FTX.FTDI_Rx(Bytes_Read);

    // Bytes_Read = 64;

    Bytes_Wrote = 0;
    if (Bytes_Read > 0)
    {
      if (Bytes_Read > 64)
        cout << "********************************   Missed Frame  ******************************* " << Bytes_Read << " Bytes Read " << endl;

      if (FTX.ftStatus == FT_OK)
      {

        // if (pauseX & !pauseX_old)
        // {
        //   VP1.player_pause = true;
        //   VP2.player_pause = true;
        // }
        // else if (!pauseX & pauseX_old)
        // {
        //   VP1.player_pause = false;
        //   VP2.player_pause = false;
        // }
        // pauseX_old = pauseX;

        FTX.FTDI_Load_TxBuffer((char *)SC1.Samples_Mapped_To_Sculpture);


        // no threading version          // NUC DELAY = 1.8 ms 
        // FTX.FTDI_Txx(); // .8 ms
        // SC1.Play_All(); // 1.3 ms             


        // threading version     // NUC DELAY = 2 ms
        std::thread t1(&Video_Sculpture::Play_All, &SC1);
        std::thread t2(&USB_FTDI_Channel::FTDI_Txx, &FTX);
        t1.join();
        t2.join();



        // NUC DELAY = 9.5 ms NUC
        Time_Delay_2.Start_Delay_Timer();             
        SC1.Mixer();
        Time_Delay_2.End_Delay_Timer();           


      // NUC DELAY = .34 ms NUC
        SC1.Multi_Map_Image_To_Sculpture();

    

        // Time_Delay_2.Start_Delay_Timer();
      // NUC DELAY = .67 ms NUC        
        SC1.Display();
        // Time_Delay_2.End_Delay_Timer();            

        Process_Time.End_Delay_Timer();

        Time_Delay_1.End_Delay_Timer();
        loop++;
        if (1) // loop % 30 == 0)
        {
          // if (Time_Delay_1.time_delay > 8)
          //   cout << "   " << Time_Delay_1.time_delay_max << "             ";
          cout << "   Loop_Time cur/avg/max/min: " << std::setprecision(2) << setw(4) << Time_Delay_1.time_delay;
          cout << " " << std::setprecision(2) << setw(4) << Time_Delay_1.time_delay_avg;
          cout << " " << std::setprecision(2) << setw(4) << Time_Delay_1.time_delay_max;
          cout << " " << std::setprecision(2) << setw(4) << Time_Delay_1.time_delay_min;

          if (Process_Time.time_delay > 16)
            cout << " " << Process_Time.time_delay_max << "             ";
          cout << "   Process_Time cur/avg/max/min: " << std::setprecision(2) << setw(4) << Process_Time.time_delay;
          cout << " " << std::setprecision(2) << setw(4) << Process_Time.time_delay_avg;
          cout << " " << std::setprecision(2) << setw(4) << Process_Time.time_delay_max;
          cout << " " << std::setprecision(2) << setw(4) << Process_Time.time_delay_min;

          cout << "    display time: " << Time_Delay_2.time_delay << "  " << Time_Delay_2.time_delay_avg;
          cout << "    waitkey time: " << Time_Delay_3.time_delay;

          cout << "    frame#: " << loop << std::endl;
        }
        Time_Delay_1.Start_Delay_Timer();

        Time_Delay_3.Start_Delay_Timer();

        unsigned char c = 255;
        c = (unsigned char)waitKey(1);

        SC1.KeyBoardInput(c, Finished);
        if (Finished)
          break;

        // if (c != -1)
        // {
        //   if (c == 27)
        //     Finished = true;
        //   else if ((c == 99) & (last_c == 227)) // "Cntrl C"
        //   {
        //     cout << "should be done " << endl;
        //     Finished = true;
        //   }
        //   else if (c == 'p')
        //     pauseX = !pauseX;
        //   else if (c == 'd')
        //   {
        //     SC1.display_on_X = !SC1.display_on_X;
        //   }        FTX.FTDI_Txx(); // TxBuffer, Bytes_Wrote)
        SC1.Play_All();
        // if (Finished)
        //   break;

        Time_Delay_3.End_Delay_Timer();
      }
    }

    // not sure why this helps  maybe the FTDI read function being read so many times causes issues
    else
      Delay_Msec(.12); // .25);
  }

  destroyAllWindows();

  return 0;
}
