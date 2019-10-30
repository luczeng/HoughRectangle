import matplotlib.pyplot as plt
import argparse
import numpy as np
import cv2

parser = argparse.ArgumentParser()
parser.add_argument("--input","-i",help = "path to input image",type = str)
args = vars(parser.parse_args())

img=cv2.imread(args["input"])

plt.imshow(img)
plt.show()
