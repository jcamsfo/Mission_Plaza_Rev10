#ifndef FILE_IO_2_H
#define FILE_IO_2_H
#pragma once

// #include <iostream>
// #include <cstdio>
// #include <time.h>
// #include <ctime>
// #include <chrono>
// #include <fstream>
// #include <sstream>
// #include <thread>
// #include <vector>
// #include <iomanip> 
// #include <algorithm>
// #include <cmath>    
// #include <functional>   // std::plus
// #include <string>


#include "measure2.h"
#include <boost/algorithm/string/replace.hpp>





bool Read_1D(vector <int> & Vec_1Dim,  string file_name);
bool Read_2D(vector < vector<int> > & Vec_2Dim, string filename);

bool Read_2D_Ignore(vector<vector<int>> &Vec_2Dim, string file_name);
bool Read_2D_Ignore(vector<vector<int>> &Vec_2Dim, string file_name, int Num_of_Ignore_Lines);

int Read_YAML_Data(const char *Filename,  std::string  FileData[][12]   );


// reads  format of all ints
// void Read_1D(vector<int> Vec_1Dim, string file_name);



// void Write_1D(const std::vector <float >& Vec_1Dim, string filename);

// void Write_2D(const vector < vector<float> >& Vec_2Dim, string filename);
// void Write_2D(const std::vector < std::vector<int> >& Vec_2Dim, string filename);

// // void Read_1D(vector<float>& Vec_1Dim, string filename);

// void Read_1D(vector<int> Vec_1Dim, string file_name);

// void Read_1Dxx(vector<int> & Vec_1Dim);

// void Read_1Dxxx(void);



// bool Read_2Di(vector < vector<int> >& Vec_2Dim, string filename);

// void Read_1Dx(vector<float>& Vec_1Dim, string filename);

// void Read_1Dz(vector<float>& Vec_1Dim, string filename);
// void Read_1Dz(vector<int>& Vec_1Dim, string filename);


// void Write_Line_History(const vector < vector<float> >& Vec_2Dim);
// void string_test(void);

#endif /* FILE_IO_2_H */

