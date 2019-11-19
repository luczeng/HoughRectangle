# Hough rectangle detection
## Intro
This is a personal project which aim is to implemenent a rectangle detection algorithm using the Hough transform from the paper ["Rectangle Detection based on a Windowed Hough Transform"](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.59.4239&rep=rep1&type=pdf) from C.Jung and R.Schramm. The Hough rectangle detection is based on detecting specific patterns in the Hough line domain of an image. The algorithm relies on a windowed Hough transform to achieve robustness.

The implementation is done in c++ and is intended to be lightweight, ie no image processing library is used. We mostly rely on "Eigen" for image manipulation.  

The work is currently in progress. Stay tuned for progress!

## Requirements
- cmake >3.11.4

## Usage
```
mkdir build 
cd build
cmake ..
make
./HoughRectangle -i ../img/LenaBinary.jpg -o output_img.jpg
```

Modify parameters algorithms parameters in src/configs.json 

## Project status:
- Hough transform : done
- Enhanced Hough transform : done
- Windowed Hough transform : done

