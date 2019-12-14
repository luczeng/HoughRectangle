import matplotlib.pyplot as plt
import argparse
import numpy as np
import matplotlib.image as mpimg


parser = argparse.ArgumentParser()
parser.add_argument('-m','--maximum_file',help = "path to txt file containing maximums")
parser.add_argument('-i','--input_path',help = "path to txt file containing maximums")
args = parser.parse_args()

x = np.loadtxt(args.maximum_file)

plt.imshow(mpimg.imread(args.input_path))
for i in range(x.shape[0]):
    print(i)
    plt.plot( x[i,0::2],x[i,1::2],'rx')
    plt.show()
