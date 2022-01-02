#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include"imerode.h"
using namespace cv;
using namespace std;

// Driver Code
void imerode(cv::Mat image, Mat& erod, Mat& dill)
{

    int morph_size = 2;
    Mat element = getStructuringElement(
        MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));

    // For Erosion
    erode(image, erod, element,
        Point(-1, -1), 1);

    // For Dilation
    dilate(image, dill, element,
        Point(-1, -1), 1);

}