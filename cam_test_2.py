import cv2

cap = cv2.VideoCapture(0)
cap.set(3, 320)
cap.set(4, 240)

capB = cv2.VideoCapture(1)
capB.set(3, 320)
capB.set(4, 240)

while True:
   ret, img = cap.read()
   cv2.imshow("CameraA", img)

   key = cv2.waitKey(20)
   if key == 27:
       break

   ret, imgB = capB.read()
   cv2.imshow("CameraB", imgB)

   key = cv2.waitKey(20)
   if key == 27:
       break

cv2.destroyAllWindows()
cv2.VideoCapture(0).realease()
cv2.VideoCapture(1).realease()
