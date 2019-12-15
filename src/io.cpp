#include "io.hpp"
#include <Eigen/Dense>
#include <iostream>
#include "stb_image_write.h"
#include "stb_image.h"
#include "string"
#include <fstream>
#include <array>
#include <cmath>


// TODO(luczeng): use specific using clauses
using namespace Eigen;

/**
 * Converts raw buffer to Eigen float matrix
 *
 * \param data unsigned char raw buffer to be converted
 * \param x image dimension in x direction
 * \param y image dimension in y direction
 *
 *  \return gray eigen float matrix
 */
// TODO(luczeng): why do you pass in a unique_ptr by reference? Just pass the pointer, passing the unique pointer does not give you any extra advantage, and it is confusing.
//                std::unique_ptr are very useful inside classes, not so much as arguments to functions.
// TODO(luczeng): use return type for gray, not output argument.
void convert_RawBuff2Mat(std::unique_ptr<unsigned char[]> &data, MatrixXf &gray,
                         int x, int y) {
    const int nx = x;
    const int ny = y;
    typedef Matrix<unsigned char, Dynamic, Dynamic, RowMajor> MatrixXUC;
    typedef Map<MatrixXUC> MapType;
    MapType img(data.get(), y, x);
    gray = img.cast<float>();
}

/**
 * Converts Eigen matrix to raw buffer
 *
 * \param gray input float eigen matrix in RowMajor order
 * \param size integer total size of the raw buffer (x*y)
 *
 *  \return gray_UC unique_ptr to uc raw buffer output
 */
void convert_Mat2RawBuff(Matrix<float, Dynamic, Dynamic, RowMajor> gray,
                         std::unique_ptr<unsigned char[]> &gray_UC, int size) {
    // gray_UC = new unsigned char[size];
    for (int i = 0; i < size; ++i) {
        gray_UC[i] = static_cast<unsigned char>(round(*(gray.data() + i)));
    }
}

/**
 * Saves Eigen matrix to png
 *
 * \param img input float eigen matrix in RowMajor order
 * \param filename output file path ending in .png
 * \param x int, x dimension
 * \param y int, y dimension
 *
 */
// TODO(luczeng): don't make a copy of 'img' here, it will cost you a lot of time if the Matrix is big. Better pass it by reference or by pointer.
int save_image(MatrixXf img, std::string filename, int size, int x, int y) {
    // Normalise to 0-255
    img = img / img.maxCoeff() * 255;
    img = img.array().ceil();

    // Convert to raw buffer
    std::unique_ptr<unsigned char[]> gray_UC_hough(new unsigned char[size]);
    convert_Mat2RawBuff(img, gray_UC_hough, size);

    return stbi_write_png(filename.c_str(), x, y, 1, gray_UC_hough.get(), x);
}

/**
 * Loads png image to Eigen matrix
 *
 * \param img input float eigen matrix in RowMajor order
 * \param filename output file path ending in .png
 *
 */
MatrixXf read_image(std::string filename) {
    // Load data to raw buffer
    int x,y,n; 
    std::unique_ptr<unsigned char[]> data(
        stbi_load(filename.c_str(), &x, &y, &n, 0));

    if (data == NULL) {
        // TODO(luczeng): std::cerr instead of std::cout?
        std::cout << "Can't load image" << std::endl;
    }

    // Convert to raw buffer
    Eigen::MatrixXf img;
    convert_RawBuff2Mat(data,img,x,y);

    return img;
}

/*
 * Saves maximums detected position in text file
 *
 */
void save_maximum(std::string filename,std::vector<std::array<int,2>> indexes) {

    std::ofstream maximums(filename.c_str());

    if (maximums.is_open()){
        for (int i = 0; i <indexes.size(); ++i) {
           maximums <<indexes[i][0]<<" "<<indexes[i][1]<<"\n";
        }
        maximums.close();
    }
    else
        std::cerr<<"Couldnt save maximum file"<<std::endl;
}

/*
 * Saves maximums detected position in text file
 * T0D0: template and merge with previous func
 *
 */
void save_maximum_values(std::string filename,std::vector<std::array<float,2>> indexes) {

    std::ofstream maximums(filename.c_str());

    if (maximums.is_open()){
        for (int i = 0; i <indexes.size(); ++i) {
           maximums <<indexes[i][0]<<" "<<indexes[i][1]<<"\n";
        }
        maximums.close();
    }
    else
        std::cerr<<"Couldnt save maximum file"<<std::endl;
}

/*
 * Convert normal coordinate to cartesian coordinates
 *
 *
 * \param normal line in normal form rho, theta
 * \return cartesian ax + by + c = 0, a,b,c
 */
std::array<float,3> convert_normal2cartesian(float angle, float rho){

    std::array<float,3> cartesian;

    cartesian[0] = cos((180-angle)*M_PI/ 180.0);
    cartesian[1] = sin((180-angle)*M_PI/ 180.0);
    cartesian[2] = -rho;
    
    return cartesian;
}

/*
 * Convert normal rectangle into corner format
 */
std::array<int,8> convert_normal_rect2_corners_rect(std::array<float,3> in_rectangle, float x_bias, float y_bias){

    std::array<int,8> rectangle;

    //Create rectangle lines
    float bias;
    if (in_rectangle[0] < 0)
         bias = 90;
    else
         bias = -90;

    std::array<float,3> line1= convert_normal2cartesian(in_rectangle[0],in_rectangle[1]);
    std::array<float,3> line2= convert_normal2cartesian(in_rectangle[0],-in_rectangle[1]);
    std::array<float,3> line3= convert_normal2cartesian(in_rectangle[0] +bias,in_rectangle[2]);
    std::array<float,3> line4= convert_normal2cartesian(in_rectangle[0] +bias,-in_rectangle[2]);

    //Compute rectangle corners
    rectangle[0] = (-line1[2]*line3[1] + line1[1]*line3[2]) / (line1[0]*line3[1] - line1[1]*line3[0])+ x_bias;
    rectangle[1] = -(-line1[0]*line3[2] + line1[2]*line3[0]) / (line1[0]*line3[1] - line1[1]*line3[0])+ y_bias;
 
    rectangle[2] = (-line1[2]*line4[1] + line1[1]*line4[2]) / (line1[0]*line4[1] - line1[1]*line4[0])+ x_bias;
    rectangle[3] = -(-line1[0]*line4[2] + line1[2]*line4[0]) / (line1[0]*line4[1] - line1[1]*line4[0])+ y_bias;
 
    rectangle[4] = (-line2[2]*line3[1] + line2[1]*line3[2]) / (line2[0]*line3[1] - line2[1]*line3[0])+ x_bias;
    rectangle[5] = -(-line2[0]*line3[2] + line2[2]*line3[0]) / (line2[0]*line3[1] - line2[1]*line3[0])+ y_bias;
 
    rectangle[6] = (-line2[2]*line4[1] + line2[1]*line4[2]) / (line2[0]*line4[1] - line2[1]*line4[0])+ x_bias;
    rectangle[7] = -(-line2[0]*line4[2] + line2[2]*line4[0]) / (line2[0]*line4[1] - line2[1]*line4[0])+ y_bias;

    return rectangle;
}

/*
 * Convert all rectangles to corner format
 */
std::vector<std::array<int,8>> convert_all_rects_2_cartesian(std::vector<std::array<float,3>> rectangles, float x_bias,float y_bias) {


    std::vector<std::array<int,8>> rectangles_cart;
    for (std::array<float,3> rect : rectangles)
        rectangles_cart.push_back(convert_normal_rect2_corners_rect(rect, x_bias,y_bias));

    return rectangles_cart;

}


/*
 * Saves detected pairs in text file
 */
void save_pairs(std::string filename,std::vector<std::array<float,2>> pairs) {

    std::ofstream rectangle_file(filename.c_str());

    if (rectangle_file.is_open()){
        for (int i = 0; i <pairs.size(); ++i) {
           rectangle_file<<pairs[i][0]<<" "<<pairs[i][1]<<"\n";
        }
        rectangle_file.close();
    }
    else
    {std::cerr<<"Couldnt save pairs file"<<std::endl;}
}


/*
 * Saves detected rectangles in text file
 */
void save_rectangle(std::string filename,std::vector<std::array<int,8>> rectangles) {

    std::ofstream rectangle_file(filename.c_str());

    if (rectangle_file.is_open()){
        for (int i = 0; i <rectangles.size(); ++i) {
           rectangle_file<<rectangles[i][0]<<" "<<rectangles[i][1]<<" "<<rectangles[i][2]<<" "<<rectangles[i][3]<<
               " "<<rectangles[i][4]<<" "<<rectangles[i][5]<<" "<<rectangles[i][6]<<" "<<rectangles[i][7]<<"\n";
        }
        rectangle_file.close();
    }
    else
    {std::cerr<<"Couldnt save maximum file"<<std::endl;}
}
