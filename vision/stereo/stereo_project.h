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

#endif
