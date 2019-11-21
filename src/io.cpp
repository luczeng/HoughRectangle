#include "io.hpp"
#include <Eigen/Dense>
#include <iostream>
#include "stb_image_write.h"
#include "stb_image.h"
#include "string"

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
        std::cout << "Can't load image" << std::endl;
    }

    // Convert to raw buffer
    Eigen::MatrixXf img;
    convert_RawBuff2Mat(data,img,x,y);

    return img;
}
