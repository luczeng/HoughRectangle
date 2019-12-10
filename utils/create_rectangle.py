import numpy as np
import cv2

N = 256
L = 101

#Rectangle
img = np.zeros([N,N])
mid = int(N/2)
L_x = int((L-1)/1.5)
L_y = int((L-1)/2)

img[mid-L_y,mid-L_x:mid+L_x] = 255
img[mid-L_y:mid+L_y,mid+L_x] = 255
img[mid+L_y,mid-L_x:mid+L_x] = 255
img[mid-L_y:mid+L_y,mid-L_x] = 255

cv2.imwrite("rectangle1.png",img)

#Rotated rectangle
img_rotated = np.zeros([N,N])
mid = int(N/2)
L_x = int((L-1)/2)
L_y = int((L-1)/4)

img_rotated[mid-L_y,mid-L_x:mid+L_x] = 255
img_rotated[mid-L_y:mid+L_y,mid+L_x] = 255
img_rotated[mid+L_y,mid-L_x:mid+L_x] = 255
img_rotated[mid-L_y:mid+L_y,mid-L_x] = 255

rotation_mat = cv2.getRotationMatrix2D((mid,mid),60,1)
img_rotated = cv2.warpAffine(img_rotated, rotation_mat, (N,N))

cv2.imwrite("rectangle2.png",img_rotated)
