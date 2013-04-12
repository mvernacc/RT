# color_match_demo.py
# MIT Rocket Team USLI 2013 Vision System
# Demonstrates the usage of the color_match.py code

import cv
import cv2
import numpy
from color_match import Vision

vis = Vision(True)
while cv2.waitKey(10) == -1:
    print vis.detectObject()

