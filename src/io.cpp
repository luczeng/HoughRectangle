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

//-----------------------------------------------------------------------------------------------------//
void save_rectangle(const std::string &filename, const std::array<int, 8> &rectangles) {
    std::ofstream rectangle_file(filename.c_str());

    if (rectangle_file.is_open()) {
        rectangle_file << rectangles[0] << " " << rectangles[1] << " " << rectangles[2] << " " << rectangles[3] << " "
                       << rectangles[4] << " " << rectangles[5] << " " << rectangles[6] << " " << rectangles[7] << "\n";
        rectangle_file.close();
    } else {
        std::cerr << "Couldnt save maximum file" << std::endl;
    }
}
}  // namespace eigen_io
