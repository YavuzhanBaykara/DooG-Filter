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
#include "Histogram_Esitle.h"


using namespace std;
using namespace cv;



string path = "Resimler/";
string path_image = "image/";



//															Creating 0-255 histogram vector
//############################################################################################################################
void imhist(Mat image, int histogram[])
{
    for (int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }
    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            histogram[(int)image.at<uchar>(y, x)]++;
}
//############################################################################################################################






//															Display for histogram 
//############################################################################################################################
void histDisplay(int histogram[], const char* name,double &Pmaxloc, double &x1, double& x2)
{
    int hist[256];
    for (int i = 0; i < 256; i++)
    {
        hist[i] = histogram[i];
    }

    int hist_w = 512; int hist_h = 512;
    int bin_w = cvRound((double)hist_w / 256);
    Mat histImage(hist_h, hist_w, CV_8U, Scalar(255, 255, 255));


 //														    Maximum and minimum values ​​of the histogram
    int max = hist[0];
    int maxloc = 0;
    int min = hist[0];
    int minloc = 0;
    for (int i = 1; i < 256; i++) {
        if (max < hist[i]) {
            max = hist[i];
            maxloc = i;
        }
        else if (min>hist[i])
        {
            min = hist[i];
            minloc = i;
        }
    }
//############################################################################################################################





//															Display for histogram 
//############################################################################################################################
    for (int i = 0; i < 256; i++)
    {
        hist[i] = ((double)hist[i] / max) * histImage.rows;
    }
    for (int i = 0; i < 256; i++)
    {
        line(histImage, Point(bin_w * (i), hist_h), Point(bin_w * (i), hist_h - hist[i]), Scalar(0, 0, 0), 1, 8, 0);
    }

    Pmaxloc = maxloc * 2;
    x1 = 2*maxloc - 120;                                                        //triangle angle threshold value
    x2 = 120 + maxloc * 2;
 
    Point Maxloc(Pmaxloc, 0);                                                   //maximum value of triangle
    Point Minloc(x1, hist_h);                                                   //1nd value of triangle
    Point Minloc_2(x2, hist_h);                                                 //2nd value of triangle 


    cv::Scalar colorLine(50, 255, 0);                                           // Green
    int thicknessLine = 2;                                                      //line thickness

    line(histImage, Maxloc, Minloc, colorLine, thicknessLine);                  //1nd value of triangle
    
    line(histImage, Maxloc, Minloc_2, colorLine, thicknessLine);                //2nd value of triangle 
}
//############################################################################################################################




//															Histogram drawing
//############################################################################################################################
void Histogram_Draw(Mat image, double& Pmaxloc, double& x1, double& x2)
{
    int histogram[256];
    imhist(image, histogram);
    int size = image.rows * image.cols;
    float PrRk[256];

    for (int i = 0; i < 256; i++)
    {
        PrRk[i] = (double)histogram[i] / size;
    }

    float PsSk[256];

    for (int i = 0; i < 256; i++)
    {
        PsSk[i] = 0;
    }
    for (int i = 0; i < 256; i++)
        for (int j = 0; j <= i; j++)
            PsSk[i] += PrRk[j];
    int final[256];
    for (int i = 0; i < 256; i++)
        final[i] = cvRound(PsSk[i] * 255);
    for (int i = 0; i < 256; i++)
        for (int j = 0; j <= 255; j++)
            if (final[i] == final[j] && i != j)
            {
                final[i] += final[j];
            }
    int final1[256];
    for (int i = 0; i < 256; i++)
    {
        final1[i] = 0;
    }

    for (int i = 0; i < 256; i++)
    {
        final1[cvRound(PsSk[i] * 255)] = cvRound(PrRk[i] * size);
    }
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 256; j++)
            if (final1[i] == final[j] && i != j)
            {
                final1[i] += final1[j];
            }
    Mat new_image = image.clone();

    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            new_image.at<uchar>(y, x) = saturate_cast<uchar>(final[image.at<uchar>(y, x)]);
    
    histDisplay(histogram, "Original Histogram",Pmaxloc,x1,x2);
}
//############################################################################################################################