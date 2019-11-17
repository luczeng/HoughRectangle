import numpy as np
import cv2

N = 256
img = np.zeros([N,N])

L = 101;

mid = int(N/2);
x = int((L-1)/2);

img[mid - x,mid-x:mid+x] = 255
img[mid-x:mid+x,mid+x] = 255
img[mid+x,mid-x:mid+x] = 255
img[mid-x:mid+x,mid-x] = 255

cv2.imwrite("rectangle.png",img)
