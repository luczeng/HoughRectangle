#include "recursive_hough_transform.hpp"
#include <Eigen/Dense>

#define PI 3.14159265

/*************************************************************************************/
void RecursiveHoughTransform::hough_transform_vec(const std::array<Eigen::VectorXf, 2>& columns, HoughRectangle::fMat& acc,
                                         std::vector<int>& acc_col_left) {
    // Cartesian coordinate vectors
    float mid_X = round(m_L_window / 2);
    float mid_Y = round(m_L_window / 2);
    std::array<float, 2> vec_x = {-mid_X, mid_X};
    std::array<float, 2> vec_y = {-mid_Y, mid_Y};

    // Pre-compute cosines and sinuses:
    Eigen::VectorXf cosT = cos(m_theta_vec.array() * PI / 180.0);
    Eigen::VectorXf sinT = sin(m_theta_vec.array() * PI / 180.0);

    // Compute Hough transform
    int i = 0;
    for (auto column : columns) {
        for (int j = 0; j < column.size(); ++j) {
            if (column(j) != 0) {
                // generate sinusoidal curve
                for (int k = 0; k < m_theta_vec.size(); ++k) {
                    // Calculate rho value
                    float rho_tmp = (vec_x[i] * cosT[k] + vec_y[i] * sinT[k]);

                    std::vector<float>::iterator idx;
                    idx = std::lower_bound(m_rho_vec.begin(), m_rho_vec.end(), rho_tmp);  // could be replaced
                    int idx_rho = idx - m_rho_vec.begin() - 1;

                    if (idx_rho < 0) {
                        idx_rho = 0;
                    }

                    // Fill accumulator
                    acc(idx_rho, k) = acc(idx_rho, k) + 1;
                    if (i == 0) {
                        acc_col_left.push_back(idx_rho);
                        acc_col_left.push_back(k);
                    }
                    if (acc(idx_rho, k) > pow(2, 32)) {
                        std::cout << "Max value overpassed";
                    }
                }
            }
        }
        ++i;
    }
}

/*************************************************************************************/
void RecursiveHoughTransform::fast_hough_transform(HoughRectangle::fMat& acc, const std::array<Eigen::VectorXf, 2>& img_columns,
                                          std::vector<int>& acc_column) {
    // Subtract left col
    for (int i = 0; i < acc_column.size(); i += 2) {
        acc(acc_column[i], acc_column[i + 1]) -= 1;
    }
    acc_column.clear();

    // Add right col to acc
    hough_transform_vec(img_columns, acc, acc_column);
}
