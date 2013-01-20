# Grabs an image from the webcam and saves it to disk

import cv2

capture = cv2.VideoCapture()
discard, image = capture.read()
cv2.imwrite("cam_capture.jpg", image)
