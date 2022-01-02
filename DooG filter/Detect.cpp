#pragma once
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stack>
#include <vector>
#include "Detect.h"
using namespace cv;
using namespace std;


cv::Mat FindBlobs(cv::Mat& image, cv::Mat image2, cv::Mat org)
{
	std::vector<std::vector<cv::Point>> contours;

    std::vector<std::vector<cv::Point>> contours2;

    cv::Mat result;
    cv::Mat result2;


    cv::findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    cv::findContours(image2, contours2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    cv::cvtColor(org, result, COLOR_GRAY2RGB);

    int cmin = 50;
    int cmax = 10000;

    std::vector<std::vector<cv::Point>>::iterator itc = contours.begin();

    std::vector<std::vector<cv::Point>>::iterator itc2 = contours2.begin();

    while (itc != contours.end())
    {
        if (itc->size() < cmin || itc->size() > cmax)
        {
            itc = contours.erase(itc);
            
        }
        else if(itc->size() > cmin || itc->size() < cmax)
        {

            std::vector<cv::Point> pts = *itc;
            cv::Mat pointsMatrix = cv::Mat(pts);
            cv::Scalar color(0, 255, 0);

            cv::Rect r0 = cv::boundingRect(pointsMatrix);
            cv::rectangle(result, r0, color, 2);

            //-------------------------
            ++itc;
        }
    }
    while (itc2 != contours2.end())
    {
        if (itc2->size() < cmin || itc2->size() > cmax)
        {
            itc2 = contours2.erase(itc2);

        }
        else if (itc2->size() > cmin || itc2->size() < cmax)
        {

            std::vector<cv::Point> pts2 = *itc2;
            cv::Mat pointsMatrix2 = cv::Mat(pts2);
            cv::Scalar color2(0, 0, 255);

            cv::Rect r02 = cv::boundingRect(pointsMatrix2);
            cv::rectangle(result, r02, color2, 2);

            //-------------------------
            ++itc2;
        }
    }

    return result;
}

