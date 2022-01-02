#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <tchar.h>
#include<map>;
#include <conio.h>;
#include <cmath>
#include <stdio.h>
#include <direct.h>

//opencv library
#include "opencv2/opencv.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

// Header files
#include "II.h"
#include <filesystem>
#include "Histogram_Esitle.h"
#include "feature extraction.h"
#include "imerode.h"
#include "Detect.h"


typedef cv::Vec <uchar, 3> vec2b; 
typedef cv::Vec <uchar, 3> vec3b;


string* File_name = new string[1000];

namespace fs = std::filesystem;

// Main camera information
int Camera_x = 640;
int Camera_y = 480;
int sayac_sec = 0;

int main()
{
	
	Mat img;
	Mat img2;
	string path = "Görüntü Verileri/";


	//															Time values
	//############################################################################################################################
	time_t curr_time; // Hata Ayıkla -> Özellikler -> C/C++ -> Ön işlemci -> Ön İşlemci Tanımı -> ;_CRT_SECURE_NO_WARNINGS yazınız.
	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);
	int sec = tm_local->tm_sec;
	int day = tm_local->tm_mday;
	int mon = tm_local->tm_mon + 1;
	int year = tm_local->tm_year + 1900;
	std::string now = to_string(day) + "." + to_string(mon) + "." + to_string(year);			//daily date data
	string path_database = path + now;															//daily date path
	//############################################################################################################################

	//															Camera setting
	//############################################################################################################################
	VideoCapture cap(1);
	if (!cap.isOpened()) {

		cout << "Cannot open camera"<<endl;
		exit(0);
	}
	else if (cap.isOpened()) {

		cout << "Camera Is onlıne" << endl;
	}
	//############################################################################################################################
	



	//															Creating and Reading  Image Folders
	//############################################################################################################################
	int k = 0;
	bool app = false;
	for (const auto& entry : fs::directory_iterator(path))
	{
		File_name[k] = entry.path().filename().string().c_str();	//Files in the targeted folder
		if (File_name[k]==now)
		{
			app = true;
		}
		cout << File_name[k] << endl;
		k = k + 1;
	}

	if (app==false)													//If the daily file exists
	{

		const char* newpath = path_database.c_str();
		_mkdir(newpath);
	}
	else															//If the daily file exists
	{
		cout << "Günlük veri deposu oluşturulmuştur." << endl;
	}
	//############################################################################################################################



	while (true) {

		cap >> img;
		imshow("Live", img);


		//											reduced image and normal image resizing
		//############################################################################################################################
		cv::cvtColor(img, img, COLOR_RGB2GRAY);
		cv::resize(img, img2, cv::Size(100, 100), INTER_LINEAR);
		cv::resize(img, img, cv::Size(Camera_x, Camera_y), INTER_LINEAR);
		//############################################################################################################################




		//											DOOG filter processing
		//############################################################################################################################
		Mat Result = II(img);
		Mat Result2 = II(img2);
		imshow("Display window DOOG", Result);
		//############################################################################################################################

		


		//											Finding the histogram maximum and minimum value
		//############################################################################################################################
		double Pmaxloc, x1, x2;
		double Pmaxloc_2, x1_2, x2_2;
		Histogram_Draw(Result2, Pmaxloc_2, x1_2, x2_2);
		Histogram_Draw(Result, Pmaxloc, x1, x2);
		//############################################################################################################################




		//											Threshold filter of histogram triangle[Normal İmage]
		//############################################################################################################################
		for (int i = 0; i < Result.rows; i++)
		{
			for (int j = 0; j < Result.cols; j++)
			{
				if ((int)Result.at<uchar>(i, j) < x1 / 2)
				{
					Result.at<uchar>(i, j) = 0;
				}
				
				else if ((int)Result.at<uchar>(i, j) > x2 / 2 && (int)Result.at<uchar>(i, j) < 255)
				{
					Result.at<uchar>(i, j) = 255;
				}
				else
				{
					Result.at<uchar>(i, j) = 0;
				}
			}

		}
		//############################################################################################################################





		//											Threshold filter of histogram triangle[Resize İmage[100x100]]
		//############################################################################################################################
		for (int i = 0; i < Result2.rows; i++)
		{
			for (int j = 0; j < Result2.cols; j++)
			{
				if ((int)Result2.at<uchar>(i, j) < x1_2 / 2)
				{
					Result2.at<uchar>(i, j) = 0;
				}
				else if ((int)Result2.at<uchar>(i, j) > x2_2 / 2 && (int)Result2.at<uchar>(i, j) < 255)
				{
					Result2.at<uchar>(i, j) = 255;
				}
				else
				{
					Result2.at<uchar>(i, j) = 0;
				}
			}

		}
		//############################################################################################################################





		//											Dilation and erosion morphology
		//############################################################################################################################
		Mat erod, dill;
		Mat erod_2, dill_2;
		imerode(Result, erod, dill);
		imerode(Result2, erod_2, dill_2);
		//############################################################################################################################






		//											Norm image, dill image and Erod image resizing
		//############################################################################################################################
		cv::resize(Result2, Result2, size(Result), INTER_LINEAR);
		cv::resize(erod_2, erod_2, size(erod), INTER_LINEAR);
		cv::resize(dill_2, dill_2, size(dill), INTER_LINEAR);
		//############################################################################################################################





		//											Common area of ​​normal and reduced pixel values
		//############################################################################################################################
		Mat Result_Norm = Result + Result2;
		Mat Result_Erod = erod_2 + erod;
		Mat Result_dill = dill_2 + dill;
		//############################################################################################################################



		//											Target detection function
		//############################################################################################################################
		Mat detection = FindBlobs(Result_Norm, erod, img);
		//############################################################################################################################




		//											Normal image display, Resize image display, Detection target display
		//############################################################################################################################
		imshow("Detection", detection);
		imshow("Display Normal Image DOOG Filter", Result);
		imshow("Display Resize Image DOOG Filter ", Result2);
		//##############################################z##############################################################################
		




		//											İmage recording every 20 seconds
		//############################################################################################################################
		int time_shot = clock();
		time_t curr_time;
		curr_time = time(NULL);
		tm* tm_local = localtime(&curr_time);
		
		if (((int)time_shot)/1000 % 20 == 0)
		{
			cv::imwrite(path_database + "/" + now + "-" + to_string(tm_local->tm_hour)+"."+to_string(tm_local->tm_min)+"."+to_string(tm_local->tm_sec)+ ".png", detection);
			
		}
		//############################################################################################################################
		waitKey(25);

	}

	return 0;
}
