/*
 * stereo_main.cpp
 * calibration of a stereo camera pair
 * live depth map generation from stereo cameras
 *
 * Matthew Vernacchia
 * 6.s096 Intro to C/C++
 * IAP 2013
 */

#include "stereo_project.h"

int main(){
    // connect to the cameras
    VideoCapture leftCam(1);
    VideoCapture rightCam(2);
    char response[4];
    // if needed: Capture calibration images and save to disk
    printf("Would you like to capture new calibration images? (y/n): ");
    fgets(response, sizeof response, stdin);
    bool recapture = (response[0] == 'y');
    if (recapture) {
        calibCapture( leftCam, rightCam );
    }
    // if needed: calibrate the cameras
    bool recalibrate = true;
    if (recalibrate) {
        vector<string> imageList;
        bool read_ok = readStringList("calib/image_list.xml", imageList);
        if(!read_ok || imageList.empty()) {
            cout << "Failed to read calibration image list (calib/image_list.xml)" << endl;
            return 1;
        }
        FileStorage fs("calib/image_list.xml", FileStorage::READ);
        int boardSizeX = (int)fs["boardSizeX"];
        int boardSizeY = (int)fs["boardSizeY"];
        Size boardSize(boardSizeX, boardSizeY);
        fs.release();

        cout << "calibrating cameras...";
        StereoCalib(imageList, boardSize, true, true);
    }
    while(1){
        // grab images from left and right cameras
        // rectify images
        // calculate disparity
        // show depth map
        // break if exit condition
        break;
    }
    return 0;
}
