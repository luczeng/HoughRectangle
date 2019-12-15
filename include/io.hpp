#ifndef IO_HPP
#define IO_HPP
#include <Eigen/Dense>
#include <string>
#include <vector>
#include <array>

// Image io
void convert_RawBuff2Mat(std::unique_ptr<unsigned char[]> &data,
                         Eigen::MatrixXf &gray, int x, int y);
void convert_Mat2RawBuff(
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> gray,
    std::unique_ptr<unsigned char[]> &gray_UC, int size);

// TODO(luczeng): usually, you take std::strings by const reference, unless there is a good reason not to
int save_image(Eigen::MatrixXf img, std::string filename, int size, int x,
               int y);
Eigen::MatrixXf read_image(std::string filename);


// Result io
void save_maximum(std::string filename, std::vector<std::array<int ,2>> maximums);
void save_pairs(std::string filename,std::vector<std::array<float,2>> pairs);
void save_rectangle(std::string filename,std::vector<std::array<int,8>> indexes);
void save_maximum_values(std::string filename,std::vector<std::array<float,2>> indexes);

// Conversion utils
std::array<float,3> convert_normal2cartesian(float angle, float rho);
std::array<int,8> convert_normal_rect2_corners_rect(std::array<float,3> in_rectangle, float x_bias, float y_bias);
std::vector<std::array<int,8>> convert_all_rects_2_cartesian(std::vector<std::array<float,3>> rectangles, float x_bias, float y_bias);

#endif
