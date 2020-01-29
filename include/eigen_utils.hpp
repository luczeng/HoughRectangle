#include <Eigen/Dense>
#include <array>
#include <iostream>
#include <vector>

/**
 * Function to make sure binary is 0 and 255
 *
 * @param img Eigen float matrix to be normalized
 */
void normalise_img(Eigen::MatrixXf &img);

/*
 * Returns a linearly spaced array
 *
 * @param a starting value
 * @param b end value
 * @param N number of bins
 * @return vector<float>
 */
// TODO(luczeng): this is a perfect use case for a template
std::vector<float> LinearSpacedArray(const float &a, const float &b, const std::size_t &N);

/*
 * Finds position of all elements superior to threshold
 *
 * @param hough Eigen float matrix
 * @float threshold float
 * @return vector of Eigen::Index of the positions where hough is more than threshold
 */
std::vector<std::array<int, 2>> find_local_maximum(
    const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &img, const float &threshold);
