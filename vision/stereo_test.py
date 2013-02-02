# Stereo Calibration
dims=(8,5)
c=cv.CaptureFromCAM(1)
while True:
    f=cv.QueryFrame(c)
    cv.CvtColor(f,grey,cv.CV_BGR2GRAY)
    found,points=cv.FindChessboardCorners(grey,dims,cv.CV_CALIB_CB_ADAPTIVE_THRESH)
    if found!=0:
        cv.DrawChessboardCorners(f,dims,points,found)
        cv.ShowImage("win2",f)
        cv.WaitKey(10)
