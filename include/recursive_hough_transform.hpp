#include <Eigen/Dense>
#include <array>
#include <vector>
#include "process_image.hpp"

class RecursiveHoughTransform : public HoughRectangle {
    using HoughRectangle::HoughRectangle;

   public:
    void hough_transform_vec(const std::array<Eigen::VectorXf, 2>& columns, HoughRectangle::fMat& acc,
                             std::vector<int>& acc_col_left);

    void fast_hough_transform(HoughRectangle::fMat& acc, const std::array<Eigen::VectorXf, 2>& acc_col_left,
                              std::vector<int>& acc_column_left);
};
