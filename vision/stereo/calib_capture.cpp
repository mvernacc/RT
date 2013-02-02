/*
 * calib_capture.cpp
 * Capture a sequence of images from a stereo camera pair. Save these images to
 * disk for later use in calibration of the stereo camera pair
 *
 * Matthew Vernacchia
 * 6.s096 Intro to C/C++
 * IAP 2013
 */

#include "stereo_project.h"
void getNumCorners(int ncorners[] ) {
    bool gotInput = 0;
    char input[16];
    printf("This program will take sveral images of a chess board in order to calibrate the stereo cameras.\n");
    while( !gotInput ) {
        printf("Enter the number of interior corners of your chess board, in the horizontal and vertical direction, spearated by a space (example '9 6'): ");
        fgets(input, sizeof input, stdin);
        if ( sscanf(input, "%d %d", &ncorners[0], &ncorners[1]) == 2) {
            gotInput = 1;
        }
    }
    return;
}

int getNumImages() {
    bool gotInput = 0;
    char input[16];
    int n = 0;
    while( !gotInput ) {
        printf("How many calibration images would you like to capture? More images will lead to better results. : ");
        fgets(input, sizeof input, stdin);
        if( sscanf(input, "%d", &n) == 1 ) {
            gotInput = 1;
        }
    }
    return n;
}

void calibCapture(VideoCapture leftCam, VideoCapture rightCam) {
    int ncorners[2] = {0,0};
    getNumCorners(ncorners); // the dimensions of the chessboard, in terms of number of interior corners
    int nimages = getNumImages();
    printf("Press ENTER when you are ready to start capturing images\n");
    string junk;
    getline(cin, junk);
    Mat leftImage;
    Mat rightImage;
    namedWindow("Left");
    namedWindow("Right");
    char imageFileNames[nimages*2+1][64];
    time_t start, now;
    time(&start);
    double spacing = 5; // amount of time between image captures [sec]
    for(int i = 0; i < nimages; ) {
        // capture a  pair of images
        leftCam >> leftImage;
        rightCam >> rightImage;
        // every *spacing* seconds, save the pair of images to disk
        time(&now);
        if( difftime(now, start) > spacing ) {
            time(&start);            
            snprintf(imageFileNames[i*2], 64, "calib/left%d.bmp", i); 
            snprintf(imageFileNames[i*2+1], 64, "calib/right%d.bmp", i);
            imwrite(imageFileNames[i*2], leftImage);
            imwrite(imageFileNames[i*2+1], rightImage); 
            printf("Capturing image pair %d\n", i);
            waitKey(500);
            i++;
        }
        // draw a countdown showing how long untill the next image is captured on the left image
        char text[128];
        snprintf(text, 128, "Capturing image %d/%d in %1.f sec", i, nimages, ceil( spacing-difftime(now,start)));
       putText(leftImage, text, Point(10,20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,255));
        // show the pair of images. the waitKey() is needed so the GUI backend has time to draw the images.
        imshow("Left", leftImage);
        imshow("Right", rightImage);
        waitKey(10);
    }
    // save the list of the calibration image filenames, and the size of the chessboard to disk
    FileStorage fs("calib/image_list.xml", FileStorage::WRITE);
    fs << "imagelist" << "[";
        for( int i =0; i < nimages*2; i++) {
            fs << string(imageFileNames[i]);
            printf("%s\n", imageFileNames[i]);
        }
    fs << "]";
    fs << "boardSizeX" << ncorners[0] << "boardSizeY" << ncorners[1];
    fs.release();
    return;
}
