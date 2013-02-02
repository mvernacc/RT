#ifndef _STEREO_PROJECT_H
#define _STEREO_PROJECT_H
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

using namespace cv;
using namespace std;

#define IMAGE_LIST_FILE_NAME "stereo_calib_image_list.xml"

void calibCapture(VideoCapture, VideoCapture);
void StereoCalib(const vector<string>& imagelist, Size boardSize, bool useCalibrated, bool showRectified);
bool readStringList(const string&, vector<string>&);


class StereoMatcher {
private:
    StereoSGBM sgbm;
    StereoBM bm;
    StereoVar var;
    Mat M1, D1, M2, D2, R, T, Q;

    int blockSize; // must be odd
    int blockSize_default;
    int numberOfDisparities; // must be divisible by 16
    int numberOfDisparities_default;
    float scaleFactor;
    float scaleFactor_default;
    enum Algorithm {
        BM,
        SGBM,
        VAR
    } algorithm;
public:
    StereoMatcher(string intrinsicFileName, string extrinsicFileName);
    ~StereoMatcher();
    int getDisp(Mat leftImage, Mat rightImage, Mat &dispImage);
    void setParams(int blockSize_, int numberOfDisparities_, float scaleFactor_, enum Algorithm algorithm_);
};
#endif
