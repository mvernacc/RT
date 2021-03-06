# color_match.py
# MIT Rocket Team USLI 2013 Vision System
# Based on MASLab vision code by Matt Vernacchia and Adrian Li
# Identifies the location of an object in the camera's feild of view by color matching

# Import OpenCV and NumPy libraries
import cv
import cv2
import numpy as np

class Vision:
    def __init__(self, debug = False):
        self.debug = debug
        # The HSV color range to look for
        self.hsv = ( (0, 20), (60, 255), (170, 255) )

        # Set up the video camera
        self.width = 320
        self.height = 240
        self.capture = cv2.VideoCapture(0)
        self.capture.set(cv.CV_CAP_PROP_FRAME_WIDTH, self.width)
        self.capture.set(cv.CV_CAP_PROP_FRAME_HEIGHT, self.height)

        # Set up debug windows
        if self.debug:
            self.windowOriginal = "Original Image"
            self.windowExtract = "Extracted Color"
            self.windowOpened = "Opened Regions"
            self.windowContour = "Selected Contour"
            cv2.namedWindow(self.windowOriginal)
            cv2.namedWindow(self.windowExtract)
            cv2.namedWindow(self.windowOpened)
            cv2.namedWindow(self.windowContour)
            cv2.moveWindow(self.windowOriginal, 400, 50)
            cv2.moveWindow(self.windowExtract, 800, 50)
            cv2.moveWindow(self.windowOpened, 400, 400)
            cv2.moveWindow(self.windowContour, 800, 400)
        
        # Images
        self.image = None
        self.hsv_image = None

    def grabFrame(self):
        retval, self.imageBGR = self.capture.read()
        self.imageHSV = cv2.cvtColor(self.imageBGR, cv.CV_BGR2HSV)
        if self.debug:
            cv2.imshow(self.windowOriginal, self.imageBGR)

    def filterHSV(self, hue, sat, val):
        minHSV = np.array([hue[0], sat[0], val[0]], np.uint8)
        maxHSV = np.array([hue[1], sat[1], val[1]], np.uint8)
        return cv2.inRange(self.imageHSV, minHSV, maxHSV)

    def extractColor(self):
        imageExtract = self.filterHSV(self.hsv[0], self.hsv[1], self.hsv[2])
        if self.debug:
            cv2.imshow(self.windowExtract, imageExtract)
        return imageExtract

    def morphOpen(self, imageIn, size = None):
        if size == None:
            size = imageIn.shape[1] / 100
        element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (size, size))
        imageOut = cv2.erode(imageIn, element)
        imageOut = cv2.dilate(imageOut, element)
        imageOut = cv2.dilate(imageOut, element)
        if self.debug:
            cv2.imshow(self.windowOpened, imageOut)
        return imageOut

    def contourCentroid(self, imageIn):
        contours, hierarchy = cv2.findContours(imageIn, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        bestContour = None
        bestArea = 0
        result = None
        for contour in contours:
            area = cv2.contourArea(contour)
            if area > bestArea:
                bestContour = contour
                bestArea = area
        if bestContour != None:
            moments = cv2.moments(bestContour)
            # if moments["m00"] != 0:
            cx = int(moments["m10"] / moments["m00"])
            cy = int(moments["m01"] / moments["m00"])
            if self.debug:
                imageContour = np.zeros(self.imageHSV.shape, np.uint8)
                cv2.drawContours(imageContour, [bestContour], 0, (0, 255, 0), 1)
                cv2.circle(imageContour, (cx, cy), 1, (0, 0, 255), -1)
                cv2.imshow(self.windowContour, imageContour)
            result = (cx, cy, bestArea)
        return result
 
    def detectObject(self):
        """ Detect a colored object. Returns the x coordinate, y coordinate, and the area of the largest contour enclosing a region of that color"""
        # grab a frame form the imaging device
        self.grabFrame()
        # extract regions which match the color of the object we are looking for
        imageExtract = self.extractColor()
        # Perform Morphological Opening to reduce noise
        imageOpened = self.morphOpen(imageExtract)
        # Find the largest contour in the processed image. Return its coordinates and area
        return self.contourCentroid(imageOpened)
        




