#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;


//											Normalization
//############################################################################################################################
Mat norm_0_255(const Mat& src) {

    Mat dst;
    switch (src.channels()) {
    case 1:
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}
//############################################################################################################################



//											DOOG filter processing
//############################################################################################################################
Mat tan_triggs_preprocessing(InputArray src,
    float alpha = 0.1, float tau = 10.0, float gamma = 0.2) {

    Mat X = src.getMat();
    X.convertTo(X, CV_32FC1);
    Mat I;
    pow(X, gamma, I);
    {

        int sigma0 = 20;                                                                                    //Sigma0 value                                                                     
        int sigma1 = 4;                                                                                     //Sigma1 value        

        Mat gaussian0, gaussian1;
        int kernel_sz0 = (3 * sigma0);                                                                      //Generating kernel matrix for Gaussian K1
        int kernel_sz1 = (3 * sigma1);                                                                      //Generating kernel matrix for Gaussian K1
        

        kernel_sz0 += ((kernel_sz0 % 2) == 0) ? 1 : 0;
        kernel_sz1 += ((kernel_sz1 % 2) == 0) ? 1 : 0;
        GaussianBlur(I, gaussian0, Size(kernel_sz0, kernel_sz0), sigma0, sigma0, BORDER_REPLICATE);         //GaussianBlur function G1 //Read "328475" in the folder(page:54)

        GaussianBlur(I, gaussian1, Size(kernel_sz1, kernel_sz1), sigma1, sigma1, BORDER_REPLICATE);         //GaussianBlur function G2 //Read "328475" in the folder(page:54)
        subtract(gaussian0, gaussian1, I);                                                                  //G2 and G1 difference      

    }

    {
        double meanI = 0.0;
        {
            Mat tmp;
            pow(abs(I), alpha, tmp);
            meanI = mean(tmp).val[0];

        }
        I = I / pow(meanI, 1.0 / alpha);
    }

    {
        double meanI = 0.0;
        {
            Mat tmp;
            pow(min(abs(I), tau), alpha, tmp);
            meanI = mean(tmp).val[0];
        }
        I = I / pow(meanI, 1.0 / alpha);
    }

    {
        Mat exp_x, exp_negx;
        exp(I / tau, exp_x);
        exp(-I / tau, exp_negx);
        divide(exp_x - exp_negx, exp_x + exp_negx, I);
        I = tau * I;
    }
    return I;
}
//############################################################################################################################




Mat II(Mat image)
{

    Mat preprocessed = tan_triggs_preprocessing(image);
    return norm_0_255(preprocessed);

}