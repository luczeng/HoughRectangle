import matplotlib.pyplot as plt
import argparse
import numpy as np
import matplotlib.image as mpimg


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-m','--maximum_file',help = "path to txt file containing maximums")
    parser.add_argument('-i','--input_path',help = "path to txt file containing maximums")
    args = parser.parse_args()

    x = np.loadtxt(args.maximum_file)

    plt.imshow(mpimg.imread(args.input_path),cmap = 'gray')
    if x.shape[0] == 8:
        plt.plot( x[0::2],x[1::2],'rx')
    else:
        for rect in x:
            plt.plot( rect[0::2],rect[1::2],'rx')
    plt.axis('off')
    plt.show()
