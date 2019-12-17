import matplotlib.pyplot as plt
import argparse
import numpy as np
import matplotlib.image as mpimg


parser = argparse.ArgumentParser()
parser.add_argument('-m','--maximum_file',help = "path to txt file containing maximums")
parser.add_argument('-i','--input_path',help = "path to txt file containing maximums")
args = parser.parse_args()

x = np.loadtxt(args.maximum_file)
img = mpimg.imread(args.input_path)
N = 256
# print(vec_x)

plt.imshow(img,extent = [-img.shape[1]/2,img.shape[1]/2,-img.shape[0]/2,img.shape[0]/2])
for i in range(x.shape[0]):
    theta = x[i,1]
    theta = 180 - x[i,1]

    #Find line equations
    if np.abs(theta) < 2:
        vec_x = [x[i,0]]*N
        vec_y = np.linspace(-img.shape[0]/2,img.shape[0]/2,N)
    else:
        vec_x = np.arange(0,N)-N/2
        a = -1/np.tan(theta*3.14159265359/180)
        b = x[i,0]/np.sin(theta*3.14159265359/180)
        vec_y =np.round(a*(vec_x) +b)
        idx_min= min(range(len(vec_y)), key=lambda i: abs(vec_y[i]+ img.shape[0]/2))
        idx_max = min(range(len(vec_y)), key=lambda i: abs(vec_y[i]-img.shape[0]/2))

    # Find extreme points
    x_min = vec_x[idx_min]
    y_min = vec_y[idx_min]
    x_max = vec_x[idx_max]
    y_max = vec_y[idx_max]

    plt.plot( [x_min, x_max],[y_min, y_max],'r-')
plt.show()
