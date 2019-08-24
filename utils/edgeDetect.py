import argparse
import cv2

parser = argparse.ArgumentParser()
parser.add_argument("--input","-i",help = "path to input image",type = str)
parser.add_argument("--output","-o",help = "path to output input image",type = str)
args = vars(parser.parse_args())

img = cv2.imread(args["input"],0)
edges = cv2.Canny(img,50,100)

cv2.imwrite(args["output"],edges)
