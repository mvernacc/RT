# Grabs and shows video from one camera.
import cv2
capture = cv2.VideoCapture(2)
cv2.namedWindow("Camera")
while True:
    discard, image = capture.read()
    cv2.imshow("Camera", image)
    cv2.waitKey(10)
