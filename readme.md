# Hough rectangle detection
## Intro
This is a personal project which aim is to implemenent a rectangle detection algorithm using the Hough transform from the paper ["Rectangle Detection based on a Windowed Hough Transform"](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.59.4239&rep=rep1&type=pdf) from C.Jung and R.Schramm. The Hough rectangle detection is based on detecting specific patterns in the Hough line transform domain of an image. The algorithm relies on a windowed Hough transform to achieve robustness.

The implementation is done in c++ and is intended to be lightweight, ie no image processing library is used. We mostly rely on "Eigen" for image manipulation. 

The work is currently in progress. Stay tuned for progress!

## Requirements
- cmake >3.11.4

## Usage
For now, the code only accepts **_png_** images. The input image is expected to be an _edge detected image_.  

```
mkdir build 
cd build
cmake ..
make
./HoughRectangle -i ../img/LenaBinary.png-o output_img.png
```

Modify parameters algorithms parameters in src/configs.json 

Some python opencv scripts in "utils" folder are provided to facilitate usage. Require opencv and matplolib. Call `-h`for help. :  
-`detect_edges`: applies Canny edge detection algorithm to input.  
-`create_rectangle`: creates some dummy rectangles, with or without an angle.  
-`display`: calls matplotlib on input image

## Algorithm
The Hough rectangle detection algorithm relies on detecting specific patterns in the Hough domain, as illustrated below:

Rectangle            |  Corresponding Hough pattern
:-------------------------:|:-------------------------:
<img src="img/rectangle2.png" width="256" height="256">  |   <img src="img/examples/example_hough.png" width="256" height="256">


## Project status:
- Hough transform: done
- Enhanced Hough transform: done
- Windowed Hough transform: done
- Peak detection: done
- Hough rectangle detection: in progress (expected mid-december 2019)

