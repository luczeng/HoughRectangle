#include "io.hpp"
#include <Eigen/Dense>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"

using namespace Eigen;

namespace eigen_io {

void convert_RawBuff2Mat(const std::unique_ptr<unsigned char[]> &data, MatrixXf &gray, const int &x, const int &y) {
    const int nx = x;
    const int ny = y;
    typedef Matrix<unsigned char, Dynamic, Dynamic, RowMajor> MatrixXUC;
    typedef Map<MatrixXUC> MapType;
    MapType img(data.get(), y, x);
    gray = img.cast<float>();
}

//-----------------------------------------------------------------------------------------------------//
void convert_Mat2RawBuff(const Matrix<float, Dynamic, Dynamic, RowMajor> &gray,
                         std::unique_ptr<unsigned char[]> &gray_UC, const int &size) {
    // gray_UC = new unsigned char[size];
    for (int i = 0; i < size; ++i) {
        gray_UC[i] = static_cast<unsigned char>(round(*(gray.data() + i)));
    }
}

//-----------------------------------------------------------------------------------------------------//
int save_image(Matrix<float, Dynamic, Dynamic, RowMajor> &img, const std::string &filename, const int &size,
               const int &x, const int &y) {
    // Normalise to 0-255
    img = img / img.maxCoeff() * 255;
    img = img.array().ceil();

    // Convert to raw buffer
    std::unique_ptr<unsigned char[]> gray_UC_hough(new unsigned char[size]);
    convert_Mat2RawBuff(img, gray_UC_hough, size);

    return stbi_write_png(filename.c_str(), x, y, 1, gray_UC_hough.get(), x);
}

//-----------------------------------------------------------------------------------------------------//
MatrixXf read_image(const std::string &filename) {
    // Load data to raw buffer
    int x, y, n;
    std::unique_ptr<unsigned char[]> data(stbi_load(filename.c_str(), &x, &y, &n, 0));

    if (data == NULL) {
        // TODO(luczeng): std::cerr instead of std::cout?
        std::cout << "Can't load image" << std::endl;
    }

    // Convert to raw buffer
    Eigen::MatrixXf img;
    convert_RawBuff2Mat(data, img, x, y);

    return img;
}

//-----------------------------------------------------------------------------------------------------//
void save_maximum(const std::string &filename, const std::vector<std::array<int, 2>> &indexes) {
    std::ofstream maximums(filename.c_str());

    if (maximums.is_open()) {
        for (int i = 0; i < indexes.size(); ++i) {
            maximums << indexes[i][0] << " " << indexes[i][1] << "\n";
        }
        maximums.close();
    } else
        std::cerr << "Couldnt save maximum file" << std::endl;
}


//-----------------------------------------------------------------------------------------------------//
std::array<float, 3> convert_normal2cartesian(const float &angle, const float &rho) {
    std::array<float, 3> cartesian;

    cartesian[0] = cos((180 - angle) * M_PI / 180.0);
    cartesian[1] = sin((180 - angle) * M_PI / 180.0);
    cartesian[2] = -rho;

    return cartesian;
}

//-----------------------------------------------------------------------------------------------------//
std::array<int, 8> convert_normal_rect2_corners_rect(const std::array<float, 3> &in_rectangle, const float &x_bias,
                                                     const float &y_bias) {
    std::array<int, 8> rectangle;

    // Angle shift between first set of lines and second set of lines
    float bias;
    if (in_rectangle[0] < 0)
        bias = 90;
    else
        bias = -90;

    // Create rectangle lines
    std::array<float, 3> line1 = convert_normal2cartesian(in_rectangle[0], in_rectangle[1]);
    std::array<float, 3> line2 = convert_normal2cartesian(in_rectangle[0], -in_rectangle[1]);
    std::array<float, 3> line3 = convert_normal2cartesian(in_rectangle[0] + bias, in_rectangle[2]);
    std::array<float, 3> line4 = convert_normal2cartesian(in_rectangle[0] + bias, -in_rectangle[2]);

    // Compute rectangle corners
    rectangle[0] = (-line1[2] * line3[1] + line1[1] * line3[2]) / (line1[0] * line3[1] - line1[1] * line3[0]) + x_bias;
    rectangle[1] = -(-line1[0] * line3[2] + line1[2] * line3[0]) / (line1[0] * line3[1] - line1[1] * line3[0]) + y_bias;

    rectangle[2] = (-line1[2] * line4[1] + line1[1] * line4[2]) / (line1[0] * line4[1] - line1[1] * line4[0]) + x_bias;
    rectangle[3] = -(-line1[0] * line4[2] + line1[2] * line4[0]) / (line1[0] * line4[1] - line1[1] * line4[0]) + y_bias;

    rectangle[4] = (-line2[2] * line3[1] + line2[1] * line3[2]) / (line2[0] * line3[1] - line2[1] * line3[0]) + x_bias;
    rectangle[5] = -(-line2[0] * line3[2] + line2[2] * line3[0]) / (line2[0] * line3[1] - line2[1] * line3[0]) + y_bias;

    rectangle[6] = (-line2[2] * line4[1] + line2[1] * line4[2]) / (line2[0] * line4[1] - line2[1] * line4[0]) + x_bias;
    rectangle[7] = -(-line2[0] * line4[2] + line2[2] * line4[0]) / (line2[0] * line4[1] - line2[1] * line4[0]) + y_bias;

    return rectangle;
}

//-----------------------------------------------------------------------------------------------------//
std::vector<std::array<int, 8>> convert_all_rects_2_cartesian(const std::vector<std::array<float, 3>> &rectangles,
                                                              const float &x_bias, const float &y_bias) {
    std::vector<std::array<int, 8>> rectangles_cart;
    for (std::array<float, 3> rect : rectangles)
        rectangles_cart.push_back(convert_normal_rect2_corners_rect(rect, x_bias, y_bias));

    return rectangles_cart;
}

//-----------------------------------------------------------------------------------------------------//
void save_pairs(const std::string &filename, const std::vector<std::array<float, 4>> &pairs) {
    std::ofstream rectangle_file(filename.c_str());

    if (rectangle_file.is_open()) {
        for (int i = 0; i < pairs.size(); ++i) {
            rectangle_file << pairs[i][0] << " " << pairs[i][1] << "\n";
        }
        rectangle_file.close();
    } else {
        std::cerr << "Couldnt save pairs file" << std::endl;
    }
}

//-----------------------------------------------------------------------------------------------------//
void save_rectangle(const std::string &filename, const std::vector<std::array<int, 8>> &rectangles) {
    std::ofstream rectangle_file(filename.c_str());

    if (rectangle_file.is_open()) {
        for (int i = 0; i < rectangles.size(); ++i) {
            rectangle_file << rectangles[i][0] << " " << rectangles[i][1] << " " << rectangles[i][2] << " "
                           << rectangles[i][3] << " " << rectangles[i][4] << " " << rectangles[i][5] << " "
                           << rectangles[i][6] << " " << rectangles[i][7] << "\n";
        }
        rectangle_file.close();
    } else {
        std::cerr << "Couldnt save maximum file" << std::endl;
    }
}
}  // namespace eigen_io
