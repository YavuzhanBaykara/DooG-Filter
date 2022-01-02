#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "feature extraction.h"

using namespace cv;
using namespace std;



void Variance(Mat img)
{
	Scalar mean, stddev;
	meanStdDev(img, mean, stddev);
	cout << "mean: " << mean << "-------" << "stddev: " << stddev << endl;
}
void Skewness_and_Kurtosis(Mat img)
{
	// Setup input
	Scalar mean, stddev, skewness, kurtosis;

	// Skewness setup
	skewness.val[0] = 0;
	int sum_channel_Grayscale_Skewness;
	float den_channel_Grayscale_Skewness =0;

	// Kurtosis setup
	kurtosis.val[0] = 0;
	int sum_channel_Grayscale_Kurtosis;
	float den_channel_Grayscale_Kurtosis = 0;

	// Skewness and kurtosis values ​​obtained
	int N = img.cols * img.rows;
	meanStdDev(img, mean, stddev);


	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
		
			sum_channel_Grayscale_Skewness = (int)img.ptr<uchar>(i)[j] - (int)mean.val[0];				//Skewness calc
			skewness.val[0] += pow(sum_channel_Grayscale_Skewness, 3);									//Skewness calc
			den_channel_Grayscale_Skewness += pow(sum_channel_Grayscale_Skewness, 2);					//Skewness calc


			//######
			sum_channel_Grayscale_Kurtosis = (int)img.ptr<uchar>(i)[j] - (int)mean.val[0];				//kurtosis calc
			kurtosis.val[0] += pow(sum_channel_Grayscale_Kurtosis, 4);									//kurtosis calc
			den_channel_Grayscale_Kurtosis += pow(sum_channel_Grayscale_Kurtosis, 2);					//kurtosis calc
			
		}
	}
	skewness.val[0] = skewness.val[0] * sqrt(N) / (den_channel_Grayscale_Skewness * sqrt(den_channel_Grayscale_Skewness));	// output Skewness
	cout << "Skewness: " << skewness.val[0] << endl;																		// output Skewness

	kurtosis.val[0] = kurtosis.val[0] * sqrt(N) / (den_channel_Grayscale_Kurtosis * sqrt(den_channel_Grayscale_Kurtosis));	// output Kurtosis
	cout << "Kurtosis: " << kurtosis.val[0] << endl;																		// output Kurtosis
}