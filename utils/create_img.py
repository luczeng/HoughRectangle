import cv2
import numpy as np

img = np.array([[1,2,3,155],[255,3,4,5],[2,5,78,1]])

print(img)

cv2.imwrite('../unit_test/test_img.png',img)

