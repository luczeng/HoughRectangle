import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--input","-i",help = "path to input image",type = str)
args = vars(parser.parse_args())

img = mpimg.imread(args["input"])

plt.imshow(img)
plt.show()
