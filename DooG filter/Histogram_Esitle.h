#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include "opencv2/opencv.hpp"
#include <opencv2/imgcodecs.hpp>
#include <tchar.h>
#include<map>;
#include <conio.h>;
#include <cmath>
#include <stdio.h>
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;



void imhist(Mat image, int histogram[]);

void histDisplay(int histogram[], const char* name, double& Pmaxloc, double& x1, double& x2);

void Histogram_Draw(Mat image, double& Pmaxloc, double& x1, double& x2);