#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <math.h>
#include <cmath>


using namespace cv;
using namespace std;

Mat II(Mat image);
Mat tan_triggs_preprocessing(InputArray src,
    float alpha = 0.1, float tau = 10.0, float gamma = 0.2);
Mat norm_0_255(const Mat& src);