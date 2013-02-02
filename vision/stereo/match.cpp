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

StereoMatcher::StereoMatcher(string intrinsicFileName, string extrinsicFileName) {
    // Assign default values to the process parameters

    blockSize = 9; // must be odd
    blockSize_default = 9;
    numberOfDisparities = 80; // must be divisible by 16
    numberOfDisparities_default = 80;
    scaleFactor = 1.0;
    scaleFactor_default = 1.0;
    algorithm = BM;
    // Load camera parameters from intrinsics file
    FileStorage fs(intrinsicFileName, CV_STORAGE_READ);
    if(!fs.isOpened()) {
        //printf("Failed to open file %s\n", intrinsicFileName);
        cout << "Failed to open file" << intrinsicFileName << endl;
    }
    fs["M1"] >> M1;
    fs["D1"] >> D1;
    fs["M2"] >> M2;
    fs["D2"] >> D2;
    // Load camera parameters from extrinsics file
    fs.open(extrinsicFileName, CV_STORAGE_READ);
    if(!fs.isOpened()) {
        //printf("Failed to open file %s\n", extrinsicFileName);
        cout << "Failed to open file" << extrinsicFileName <<endl;
    }
    fs["R"] >> R;
    fs["T"] >> T;
}
StereoMatcher::~StereoMatcher() {}
int StereoMatcher::getDisp(Mat leftImage, Mat rightImage, Mat &dispImage) {
    // check that the left an right images are of the same size
    if( leftImage.size() != rightImage.size() ) {
        printf("Error: Images are of different sizes.");
        return -1;
    }
    Size imageSize = leftImage.size();
    // check that the images have nonzero area
    if( imageSize.area() == 0 ) {
        printf("Error: Images have no area.");
        return -1;
    }
    Rect roi1, roi2;
    // rectify the image pair
    Mat R1, P1, R2, P2;
    stereoRectify( M1, D1, M2, D2, imageSize, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, imageSize, &roi1, &roi2 );
    // Remap the image pair to compensate for the distortion of the camera lenses
    Mat map11, map12, map21, map22;
    initUndistortRectifyMap(M1, D1, R1, P1, imageSize, CV_16SC2, map11, map12);
    initUndistortRectifyMap(M2, D2, R2, P2, imageSize, CV_16SC2, map21, map22);
    remap(leftImage, leftImage, map11, map12, INTER_LINEAR);
    remap(rightImage, rightImage, map21, map22, INTER_LINEAR);
    // Perform the stereo disparity calculation algorithm
    if( algorithm == BM ) {
        cvtColor(leftImage, leftImage, CV_BGR2GRAY);
        cvtColor(rightImage, rightImage, CV_BGR2GRAY);

        bm.state->roi1 = roi1;
        bm.state->roi2 = roi2;
        bm.state->preFilterCap = 31;
        bm.state->SADWindowSize = blockSize;
        bm.state->minDisparity = 0;
        bm.state->numberOfDisparities = numberOfDisparities;
        bm.state->textureThreshold = 10;
        bm.state->uniquenessRatio = 15;
        bm.state->speckleWindowSize = 100;
        bm.state->speckleRange = 32;
        bm.state->disp12MaxDiff = 1;

        bm(leftImage, rightImage, dispImage);


        dispImage.convertTo(dispImage, CV_8U, 255/(numberOfDisparities*16.));
    }
    else if( algorithm == SGBM ) {
        sgbm.preFilterCap = 63;
        sgbm.SADWindowSize = blockSize;
        int cn = leftImage.channels();
        sgbm.P1 = 8*cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
        sgbm.P2 = 32*cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
        sgbm.minDisparity = 0;
        sgbm.numberOfDisparities = numberOfDisparities;
        sgbm.uniquenessRatio = 10;
        sgbm.speckleWindowSize = bm.state->speckleWindowSize;
        sgbm.speckleRange = bm.state->speckleRange;
        sgbm.disp12MaxDiff = 1;
        sgbm.fullDP = false; //Set to true to use the HH algorithm 
        sgbm(leftImage, rightImage, dispImage);

        dispImage.convertTo(dispImage, CV_8U, 255/(numberOfDisparities*16.));
    }
    else if(algorithm == VAR) {
        var.nIt = 25;
        var.minDisp = -numberOfDisparities;
        var.maxDisp = 0;
        var.poly_n = 3;
        var.poly_sigma = 0.0;
        var.fi = 15.0f;
        var.lambda = 0.03f;
        var.flags = var.USE_SMART_ID | var.USE_AUTO_PARAMS | var.USE_INITIAL_DISPARITY | var.USE_MEDIAN_FILTERING ;
        
        var(leftImage, rightImage, dispImage);

        dispImage.convertTo(dispImage, CV_8U);
    }
    return 0;
}

void StereoMatcher::setParams(int blockSize_, int numberOfDisparities_, float scaleFactor_, enum Algorithm algorithm_) {
    if( blockSize_ < 1 || blockSize_ % 2 != 1) {
        printf("StereoMatcher::setParams(): blockSize must be a positive odd integer. Defaulting to %d\n", blockSize_default);
        blockSize = blockSize_default;
    }
    else blockSize = blockSize_;
    
    if( numberOfDisparities_ < 1 || numberOfDisparities_ % 16 != 0 ) {
        printf("StereoMatcher::setParams(): numberOfDisparities must be a positive integer divisible by 16. Defaulting to %d\n", numberOfDisparities_default);
        numberOfDisparities = numberOfDisparities_default;
    }
    else numberOfDisparities = numberOfDisparities_;

    scaleFactor = scaleFactor_;

    algorithm = algorithm_;
}

    
