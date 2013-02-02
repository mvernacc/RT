/*
 * match.cpp
 * Matching Stereo Images - converts a pair of stereo images into a disparity map
 *
 * Matthew Vernacchia
 * Intro to C/C++
 * IAP 2013
 *
 * getDisp() method is based off of examples provided for OpenCV by Victor Eruhimov.
 */

#include "stereo_project.h"

class StereoMatcher {
private:
    StereoSGBM sgbm;
    StereoBM bm;
    StereoVar var;
    Mat M1, D1, M2, D2, R, T, Q;

    int blockSize; // must be odd
    int maxDisparity; // must be divisible by 16
    float scaleFactor;
    enum Algorithm {
        BM,
        SGBM,
        VAR
    } algorithm;
public:
    StereoMatcher(char* intrinsicFileName, char* extrinsicFileName);
    ~StereoMatcher();
    int getDisp(Mat leftImage, Mat rightImage, Mat dispImage);
    void setParams(int blockSize, int maxDisparity, float scaleFactor, enum Algorithm);
}
StereoMatcher::StereoMatcher(char* intrinsicFileName, char* extrinsicFileName) {
    // Load camera parameters from intrinsics file
    FileStorage fs(intrinsicFileName, CV_STORAGE_READ);
    if(!fs.isOpened()) {
        printf("Failed to open file %s\n", intrinsicFileName);
        return -1;
    }
    fs["M1"] >> M1;
    fs["D1"] >> D1;
    fs["M2"] >> M2;
    fs["D2"] >> D2;
    // Load camera parameters from extrinsics file
    fs.open(extrinsicFileName, CV_STORAGE_READ);
    if(!fs.isOpened()) {
        printf("Failed to open file %s\n", extrinsicFileName);
        return -1;
    }
    fs["R"] >> R;
    fs["T"] >> T;
}
StereoMatcher::~StereoMatcher() {}
int StereMatcher::getDisp(Mat leftImage, Mat rightImage, Mat dispImage) {
    // check that the left an right images are of the same size
    if( leftImage.size() != rightImage.size() ) {
        printf("Error: Images are of different sizes.");
        return -1;
    }
    Size imgSize = leftImage.size();
    // check that the images have nonzero area
    if( imgSize.area() = 0 ) {
        printf("Error: Images have no area.");
        return -1;
    }
    Rect roi1, roi2;
    // rectify the image pair
    Mat R1, P1, R2, P2;
    stereoRectify( M1, D1, M2, D2, img_size, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, img_size, &roi1, &roi2 );
    // Remap the image pair to compensate for the distortion of the camera lenses
    Mat map11, map12, map21, map22;
    initUndistortRectifyMap(M1, D1, R1, P1, img_size, CV_16SC2, map11, map12);
    initUndistortRectifyMap(M2, D2, R2, P2, img_size, CV_16SC2, map21, map22);
    remap(img1, img1, map11, map12, INTER_LINEAR);
    remap(img2, img2, map21, map22, INTER_LINEAR);
    // Perform the stereo disparity calculation algorithm
    if( algorithm == BM ) {
        bm.state->roi1 = roi1;
        bm.state->roi2 = roi2;
        bm.state->preFilterCap = 31;
        bm.state->SADWindowSize = SADWindowSize > 0 ? SADWindowSize : 9;
        bm.state->minDisparity = 0;
        bm.state->numberOfDisparities = numberOfDisparities;
        bm.state->textureThreshold = 10;
        bm.state->uniquenessRatio = 15;
        bm.state->speckleWindowSize = 100;
        bm.state->speckleRange = 32;
        bm.state->disp12MaxDiff = 1;

        bm(leftImage, rightImage, dispImage);
    }
    else if( algorithm == SGBM ||
