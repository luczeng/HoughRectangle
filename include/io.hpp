#ifndef IO_HPP
#define IO_HPP
#include <Eigen/Dense>
#include <array>
#include <string>
#include <vector>

// Image io
/**
 * Converts raw buffer to Eigen float matrix
 *
 * \param data unsigned char raw buffer to be converted
 * \param x image dimension in x direction
 * \param y image dimension in y direction
 *
 *  \return gray eigen float matrix
 */
void convert_RawBuff2Mat(const std::unique_ptr<unsigned char[]> &data, Eigen::MatrixXf &gray, const int &x,
                         const int &y);

/**
 * Converts Eigen matrix to raw buffer
 *
 * \param gray input float eigen matrix in RowMajor order
 * \param size integer total size of the raw buffer (x*y)
 *
 *  \return gray_UC unique_ptr to uc raw buffer output
 */
void convert_Mat2RawBuff(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &gray,
                         std::unique_ptr<unsigned char[]> &gray_UC, const int &size);

/**
 * Saves Eigen matrix to png
 *
 * \param img input float eigen matrix in RowMajor order
 * \param filename output file path ending in .png
 * \param x int, x dimension
 * \param y int, y dimension
 *
 */
int save_image(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &img, const std::string &filename,
               const int &size, const int &x, const int &y);
/**
 * Loads png image to Eigen matrix
 *
 * \param img input float eigen matrix in RowMajor order
 * \param filename output file path ending in .png
 *
 */
Eigen::MatrixXf read_image(const std::string &filename);

// Result io
void save_maximum(const std::string &filename, const std::vector<std::array<int, 2>> &maximums);
void save_pairs(const std::string &filename, const std::vector<std::array<float, 2>> &pairs);
void save_rectangle(const std::string &filename, const std::vector<std::array<int, 8>> &indexes);
void save_maximum_values(const std::string &filename, const std::vector<std::array<float, 2>> &indexes);

// Conversion utils
std::array<float, 3> convert_normal2cartesian(const float &angle, const float &rho);
std::array<int, 8> convert_normal_rect2_corners_rect(const std::array<float, 3> &in_rectangle, const float &x_bias,
                                                     const float &y_bias);
std::vector<std::array<int, 8>> convert_all_rects_2_cartesian(const std::vector<std::array<float, 3>> &rectangles,
                                                              const float &x_bias, const float &y_bias);

#endif
