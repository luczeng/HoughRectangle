## Intro
This is a personal project which aim is to implemenent a rectangle detection algorithm using the Hough transform from the papek ["Rectangle Detection based on a Windowed Hough Transform"](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.59.4239&rep=rep1&type=pdf) from C.Jung and R.Schramm.  
The implementation is done in c++ and is intended to be lightweight, ie no image processing library is used. We mostly rely on "Eigen" for image manipulation.  

The work is currently in progress. Stay tuned for progress!

## Usage
```
mkdir build 
cd build
cmake ..
make
./HoughRectangle -i ../img/LenaBinary.jpg -o output_img.jpg
```

Modify parameters in configs.json 

## Project status:
Conventional Hough transform done!

## FAQ:
Why don't I use easier libraries like opencv? Because I simply don't want ! 
