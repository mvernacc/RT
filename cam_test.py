# Grabs and shows video from one camera.
import cv2
capture = cv2.VideoCapture(1)
cv2.namedWindow("Camera")
#captureB = cv2.VideoCapture(0)
#cv2.namedWindow("CameraB")
while True:
    discard, image = capture.read()
    cv2.imshow("Camera", image)
   # while True:
   #    discard, imageB = captureB.read()
   #    if imageB is not None:
   #       print 'B'
   #       break   
   # cv2.imshow("CameraB",imageB)
    cv2.waitKey(30)
