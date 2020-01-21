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
L_x = int((L-1)/1.2)
L_y = int((L-1)/4)

img_rotated[mid-L_y,mid-L_x:mid+L_x] = 255
img_rotated[mid-L_y:mid+L_y,mid+L_x] = 255
img_rotated[mid+L_y,mid-L_x:mid+L_x] = 255
img_rotated[mid-L_y:mid+L_y,mid-L_x] = 255

rotation_mat = cv2.getRotationMatrix2D((mid,mid),60,1)
img_rotated = cv2.warpAffine(img_rotated, rotation_mat, (N,N))

img_rotated[img_rotated>=128] =255
img_rotated[img_rotated<128] = 0

cv2.imwrite("rectangle2.png",img_rotated)

#Non centered rectangle
N = 64
mid_x = 16
mid_y = 16

L_x = 13
L_y = 8
img_non_centered = np.zeros([N,N])
img_non_centered[mid_y-L_y,mid_x-L_x:mid_x+L_x] = 255
img_non_centered[mid_y-L_y:mid_y+L_y,mid_x+L_x] = 255
img_non_centered[mid_y+L_y,mid_x-L_x:mid_x+L_x] = 255
img_non_centered[mid_y-L_y:mid_y+L_y,mid_x-L_x] = 255

cv2.imwrite("rectangle3.png",img_non_centered)

# Small rotated image
L = 32
img_rotated = np.zeros([N,N])
mid = int(N/2)
L_x = int((L-1)/2.5)
L_y = int((L-1)/6)

img_rotated[mid-L_y,mid-L_x:mid+L_x] = 255
img_rotated[mid-L_y:mid+L_y,mid+L_x] = 255
img_rotated[mid+L_y,mid-L_x:mid+L_x] = 255
img_rotated[mid-L_y:mid+L_y,mid-L_x] = 255

rotation_mat = cv2.getRotationMatrix2D((mid,mid),60,1)
img_rotated = cv2.warpAffine(img_rotated, rotation_mat, (N,N))

img_rotated[img_rotated>=128] =255
img_rotated[img_rotated<128] = 0

cv2.imwrite("rectangle4.png",img_rotated)

#Non centered rectangle bottom right
N = 64
mid_x = 40
mid_y = 48

L_x = 13
L_y = 8
img_non_centered = np.zeros([N,N])
img_non_centered[mid_y-L_y,mid_x-L_x:mid_x+L_x] = 255
img_non_centered[mid_y-L_y:mid_y+L_y,mid_x+L_x] = 255
img_non_centered[mid_y+L_y,mid_x-L_x:mid_x+L_x] = 255
img_non_centered[mid_y-L_y:mid_y+L_y,mid_x-L_x] = 255

cv2.imwrite("rectangle5.png",img_non_centered)
