
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
#include "file_io_2.h"

using namespace std;

/* defines: */
//const int H_Screen_Size = 1820;
//const int V_Screen_Size = 980;
//
//const int Canvas_Width = 1500;
//const int Canvas_Height = 300;
//const int Canvas_Location_X = 200;
//const int Canvas_Location_Y = 600;
//
//const int Canvas_Location_Right_X = (Canvas_Location_X + Canvas_Width) - 1;
//const int Canvas_Location_Bottom_Y = (Canvas_Location_Y + Canvas_Height) - 1;
//;
//const int Canvas_Channels = 3;
//const int Canvas_Width_Total = Canvas_Width * Canvas_Channels;
//const int Canvas_Total = Canvas_Width * Canvas_Height * Canvas_Channels;
//
//const int Width = 0;
//const int Height = 1;
//
//const int xloc = 0;
//const int yloc = 1;

// converts floato a text file

// const vector<int>& vect



// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \"\t\n\r\f\v{}[]")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \"\t\n\r\f\v{}[]")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \"\t\n\r\f\v{}[]")
{
    s.erase(0, s.find_first_not_of(t));
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// copying versions

inline std::string ltrim_copy(std::string s, const char* t = " \"\t\n\r\f\v{}[]")
{
    return ltrim(s, t);
}

inline std::string rtrim_copy(std::string s, const char* t = " \"\t\n\r\f\v{}[]")
{
    return rtrim(s, t);
}

inline std::string trim_copy(std::string s, const char* t = " \"\t\n\r\f\v{}[]")
{
    return trim(s, t);
}


//reads a 1D or 2D csv or text file of integers ignoring commas  puts it into a 1D vector
bool Read_1D(vector<int> &Vec_1Dim, string file_name)
{
	int Val;
	string line;

	Vec_1Dim.clear();
	std::ifstream file(file_name);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			// replace all commass csv file with spaces
			boost::replace_all(line, ",", " ");

			istringstream iss(line);

			while (iss >> Val)
			{
				Vec_1Dim.push_back(Val);
			}
		}
		file.close();
		return true;
	}
	else
	{
		cout << endl
			 << "Error opening file  " << file_name << endl;
		return false;
	}
}

//reads a 1D or 2D csv or text file of floats ignoring commas  puts it into a 1D vector
bool Read_1D(vector<float> &Vec_1Dim, string file_name)
{
	float Val;
	string line;

	Vec_1Dim.clear();
	std::ifstream file(file_name);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			// replace all commass csv file with spaces
			boost::replace_all(line, ",", " ");

			istringstream iss(line);

			while (iss >> Val)
			{
				Vec_1Dim.push_back(Val);
			}
		}
		file.close();
		return true;
	}
	else
	{
		cout << endl
			 << "Error opening file  " << file_name << endl;
		return false;
	}
}



//reads a 2D csv or text file of integers ignoring commas  puts it into a 2D vector
bool Read_2D(vector<vector<int>> &Vec_2Dim, string file_name)
{
	int Val;
	vector<int> Vec_1D;
	string line;

	Vec_2Dim.clear();
	std::ifstream file(file_name);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			// replace all commass csv file with spaces
			boost::replace_all(line, ",", " ");

			istringstream iss(line);
			Vec_1D.clear();
			while (iss >> Val)
			{
				Vec_1D.push_back(Val);
			}
			if (Vec_1D.size() > 0)
				Vec_2Dim.push_back(Vec_1D);
		}
		file.close();
		return true;
	}
	else
	{
		cout << endl
			 << "Error opening file  " << file_name << endl;
		return false;
	}
}


//reads a 2D csv or text file of floats ignoring commas  puts it into a 2D vector
bool Read_2D(vector<vector<float>> &Vec_2Dim, string file_name)
{
	float Val;
	vector<float> Vec_1D;
	string line;

	Vec_2Dim.clear();
	std::ifstream file(file_name);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			// replace all commass csv file with spaces
			boost::replace_all(line, ",", " ");

			istringstream iss(line);
			Vec_1D.clear();
			while (iss >> Val)
			{
				Vec_1D.push_back(Val);
			}
			if (Vec_1D.size() > 0)
				Vec_2Dim.push_back(Vec_1D);
		}
		file.close();
		return true;
	}
	else
	{
		cout << endl
			 << "Error opening file  " << file_name << endl;
		return false;
	}
}



//reads a 2D csv or text file of integers ignoring commas  puts it into a 2D vector nad ignores xnumber of description lines
bool Read_2D_Ignore(vector<vector<int>> &Vec_2Dim, string file_name, int Num_of_Ignore_Lines)
{
	int Val;
	vector<int> Vec_1D;
	string line;
	int Ignore_Count = 0;

	Vec_2Dim.clear();
	std::ifstream file(file_name);

	if (file.is_open())
	{
		// ignore the first line of descriptions
		while(Ignore_Count++ < Num_of_Ignore_Lines) getline(file, line);

		while (getline(file, line))
		{
			// replace all commass csv file with spaces
			std::replace_if(line.begin(), line.end(), ::isalpha, ' ');
    		std::replace_if(line.begin(), line.end(), ::ispunct, ' ');

			istringstream iss(line);
			Vec_1D.clear();
			while (iss >> Val)
			{
				Vec_1D.push_back(Val);
			}
			if (Vec_1D.size() > 0)
				Vec_2Dim.push_back(Vec_1D);
		}
		file.close();
		return true;
	}
	else
	{
		cout << endl
			 << "Error opening file  " << file_name << endl;
		return false;
	}
}


// Yaml Parser
int Read_YAML_Data(const char *Filename,  std::string  FileData[][12]   )
{

    std::ifstream infile;
    std::string input_line;

    size_t  found_comma, found_colon, found_comment;
    bool    valid_getline;
    size_t  value_cnt = 0,
    line_cnt = 0;

    infile.open(Filename);

    valid_getline = true;
    while(valid_getline)
    {
        if(std::getline(infile, input_line) )  // end of file returns 0
        {
            found_comment = input_line.find_first_of("#");  // is there a comment
            found_colon = input_line.find_first_of(":");    // is there a colon
            while( ( ( found_colon == input_line.npos) || (found_comment < found_colon) ) && (valid_getline)  ) // no colon or comment less than colon
            {
                if(getline(infile, input_line) )      // end of file returns 0         // keep grabbing lines until line has a colon
                {
                    found_colon = input_line.find_first_of(":");    // is there a colon
                    found_comment = input_line.find_first_of("#");  // is there a comment
                }
                else valid_getline = false;   // end of file
            }

            if(valid_getline)
            {
                value_cnt = 0;
                FileData[line_cnt][value_cnt++] = trim_copy( input_line.substr (0,found_colon) );  // parameter is before the colon
                cout << FileData[line_cnt][value_cnt-1] << " aaa  " << endl  ;

                if(found_comment != input_line.npos)input_line =  input_line.substr (0,found_comment);  // get rid of post comment
                input_line =  input_line.substr(found_colon+1);  // get rid of parameter from string

                found_comma = input_line.find_first_of(",");    // look for commas
                if( found_comma==input_line.npos )
                {
                    FileData[line_cnt][value_cnt++]  = trim_copy(input_line); // no comma not found
                    cout << FileData[line_cnt][value_cnt-1] << " bbb  " << endl  ;
                }
                else  // comma found
                {
                    while (found_comma!=input_line.npos)              // goes through at least once
                    {
                        FileData[line_cnt][value_cnt++]  = trim_copy(input_line.substr (0,found_comma) );      // next data
                        cout << FileData[line_cnt][value_cnt-1] << " ccc  " << endl  ;
                        input_line = input_line.substr (found_comma+1); // get rid of next data from string
                        found_comma=input_line.find_first_of(",");      // check for another comma
                    }
                    FileData[line_cnt][value_cnt++] = trim_copy(input_line); // final data
                    cout << FileData[line_cnt][value_cnt-1] << " ddd  " << endl  ;
                }
            }
        }
        else valid_getline = false;  // end of file

        line_cnt++;
    }

    infile.close();

    for(int dd=0; dd < line_cnt-1;  dd++ )
      {
    //   for(int ee=0;  ee<value_cnt; ee++)cout  <<  FileData[dd][ee] << "   "  ;
    //    printf("\n");
        }

    return line_cnt-1;
}




// void Write_1D(const vector <float >& Vec_1Dim, string file_name)
// {
// 	int jx;
// 	ofstream f;
// 	f.exceptions(std::ofstream::failbit | std::ofstream::badbit);
// 	cout << "  failbit  failbit  " << f.failbit; //  << "f.fail  f.fail  " << f.fail;
// 	f.open(file_name);
// 	for (jx = 0; jx < Vec_1Dim.size(); jx++)
// 		f << std::setprecision(8) << (Vec_1Dim[jx]) << '\t';
// 	f << endl;
// 	f.close();
// }

// void Write_2D(const std::vector < std::vector<float> >& Vec_2Dim, string file_name)
// {
// 	int ix, jx;
// 	ofstream f;
// 	f.exceptions(std::ofstream::failbit | std::ofstream::badbit);
// 	cout << "  failbit  failbit  " << f.failbit; //  << "f.fail  f.fail  " << f.fail;
// 	f.open(file_name);
// 	for (jx = 0; jx < Vec_2Dim.size(); jx++)
// 	{
// 		for (ix = 0; ix < Vec_2Dim[jx].size(); ix++)
// 			f << std::setprecision(8) << (Vec_2Dim[jx][ix]) << '\t';
// 		f << endl;
// 	}
// 	f.close();
// }

// void Write_2D(const vector < vector<int> >& Vec_2Dim, string file_name)
// {
// 	int ix, jx;
// 	ofstream f;
// 	f.exceptions(std::ofstream::failbit | std::ofstream::badbit);
// 	f.open(file_name);
// 	for (jx = 0; jx < Vec_2Dim.size(); jx++)
// 	{
// 		for (ix = 0; ix < Vec_2Dim[jx].size(); ix++)
// 			f << std::setprecision(8) << (Vec_2Dim[jx][ix]) << '\t';
// 		f << endl;
// 	}
// 	f.close();
// }

// reads  format of all floats
// void Read_1D(vector<float>& Vec_1Dim, string file_name)
// {
// 	std::ifstream f(file_name);
// 	float Val = 0;
// 	while (f >> Val)
// 		Vec_1Dim.push_back(Val);
// 	f.close();
// }

// reads  format of all floats
// void Read_1D(vector<int> Vec_1Dim, string file_name)
// {
// 	std::ifstream f(file_name);
// 	float Val = 0;
// 	while (f >> Val)
// 		Vec_1Dim.push_back(Val);
// 	f.close();
// }

// void Read_1Dxx(vector<int> & Vec_1Dim)
// {
// 	std::ifstream f("hello.txt");
// 	float Val = 0;
// 	while (f >> Val)
// 		Vec_1Dim.push_back(Val);
// 	f.close();
// }

// void Read_1Dxxx(void)
// {
// 	cout << "ARGSHGDFHDYH" ;
// }

// reads  format where each line has text followed by data
// void Read_1Dx(vector<float>& Vec_1Dim, string file_name)
// {
// 	std::ifstream f(file_name);
// 	string Str = "";
// 	int Val = 0;
// 	while (f >> Str >> Val)
// 		Vec_1Dim.push_back(Val);
// 	f.close();
// }

// void Read_1Dz(vector<float>& Vec_1Dim, string file_name)
// {
// 	std::ifstream f(file_name);
// 	string Str = "";
// 	float Val = 0;
// 	while (!f.eof())
// 	{
// 		(f >> Str >> Val);
// 		Vec_1Dim.push_back(Val);
// 	}
// 	f.close();
// }

// void Read_1Dz(vector<int>& Vec_1Dim, string file_name)
// {
// 	std::ifstream f(file_name);
// 	string Str = "";
// 	int Val = 0;
// 	while (!f.eof())
// 	{
// 		(f >> Str >> Val);
// 		Vec_1Dim.push_back(Val);
// 	}
// 	f.close();
// }

// bool Read_2D(vector < vector<float> >& Vec_2Dim, string file_name)
// {
// 	float Val;
// 	vector <float>  Vec_1D;
// 	ifstream file(file_name);

// 	Vec_2Dim.clear();
// 	if (file.is_open())
// 	{
// 		int j = 0;
// 		string line;
// 		while (getline(file, line))
// 		{
// 			istringstream iss(line);
// 			Vec_1D.clear();
// 			while (iss >> Val)
// 				Vec_1D.push_back(Val);
// 			Vec_2Dim.push_back(Vec_1D);
// 			j++;
// 		}
// 		file.close();
// 		return true;
// 	}
// 	else
// 	{
// 		cout << endl << "Error opening file  " << file_name << endl ;
// 		return false;
// 	}
// }

// bool Read_2Di(vector < vector<int> >& Vec_2Dim, string file_name)
// {
// 	int Val;
// 	vector <int>  Vec_1D;
// 	ifstream file(file_name);

// 	Vec_2Dim.clear();
// 	if (file.is_open())
// 	{
// 		int j = 0;
// 		string line;
// 		while (getline(file, line))
// 		{
// 			istringstream iss(line);
// 			Vec_1D.clear();
// 			while (iss >> Val)
// 				Vec_1D.push_back(Val);
// 			Vec_2Dim.push_back(Vec_1D);
// 			j++;
// 		}
// 		file.close();
// 		return true;
// 	}
// 	else
// 	{
// 		cout << endl << "Error opening file  " << file_name << endl ;
// 		return false;
// 	}
// }

//void Write_Line_History(const vector < vector<float> >& Vec_2Dim)
//{
//	int ix, jx;
//	std::ofstream f;
//	f.exceptions(std::ofstream::failbit | std::ofstream::badbit);
//	f.open("test20.txt");
//	for (jx = 0; jx < Vec_2Dim.size(); jx++)
//	{
//		for (ix = 0; ix < Vec_2Dim[0].size(); ix++)
//			f << std::setprecision(8) << (Vec_2Dim[jx][ix]) << '\t';
//		f << endl;
//	}
//	f.close();
//}
//
//void string_test(void)
//{
//	float Val;
//	vector <float>  Vec_1D;
//	vector <vector <float> > Vec_2D;
//
//	ifstream file("Line_History2.dat");
//	istringstream iss;
//	if (file.is_open())
//	{
//		int j = 0;
//		string line;
//		while (getline(file, line))
//		{
//			istringstream iss(line);
//			Vec_1D.clear();
//			while (iss >> Val)
//				Vec_1D.push_back(Val);
//			Vec_2D.push_back(Vec_1D);
//			j++;
//		}
//		file.close();
//
//		for (int p = 0; p < Vec_2D.size(); p++)
//		{
//			for (int q = 0; q < Vec_2D[0].size(); q++)
//				cout << std::setprecision(8) << Vec_2D[p][q] << "  ";
//			cout << endl;
//		}
//	}
//}
//
