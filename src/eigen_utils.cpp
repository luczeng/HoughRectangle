#include <Eigen/Dense>
#include <iostream>
#include "eigen_utils.hpp"

using namespace Eigen;
/*************************************************************************************/
void Log(const char* message) { std::cout << message << std::endl; }

/*************************************************************************************/
void normalise_img(Matrix<float, Dynamic, Dynamic, RowMajor>& img) {
    Matrix<float, Dynamic, Dynamic, RowMajor> high;
    high.setOnes(img.rows(), img.cols());
    high *= 255;

    Matrix<float, Dynamic, Dynamic, RowMajor> low;
    low.setZero(img.rows(), img.cols());

    Matrix<float, Dynamic, Dynamic, RowMajor> tmp = (img.array() > 128.0).select(high, low);

    img = tmp;
}

/*************************************************************************************/
std::vector<float> LinearSpacedArray(const float& a, const float& b, const std::size_t& N) {
    double h = (b - a) / static_cast<float>(N - 1);
    std::vector<float> xs(N);
    std::vector<float>::iterator x;
    float val;
    for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h) {
        *x = val;
    }
    return xs;
}

/*************************************************************************************/
std::vector<std::array<int, 2>> find_local_maximum(const Matrix<float, Dynamic, Dynamic, RowMajor>& hough,
                                                   const float& threshold) {
    std::vector<std::array<int, 2>> idxs;

    // This loop can probably be replaced by something faster(factorized?)
    // TODO(luczeng): what about std::max_element?
    // for (Index i = 0; i < hough.size(); ++i) {
    for (int i = 0; i < hough.rows(); ++i) {
        for (int j = 0; j < hough.cols(); ++j) {
            if (hough(i, j) >= threshold) {
                std::array<int, 2> x = {i, j};
                idxs.push_back(x);
            }
        }
    }

    return idxs;
}
