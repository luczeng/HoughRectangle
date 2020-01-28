#include "process_image.hpp"
#include <math.h>
#include <stdlib.h>
#include <Eigen/Dense>
#include <array>
#include <iostream>
#include <tuple>
#include "array"
#include "config.hpp"
#include "eigen_utils.hpp"
#include "io.hpp"
#include "stb_image.h"
#include "stb_image_write.h"

#define PI 3.14159265

using namespace Eigen;

/*************************************************************************************/
HoughRectangle::HoughRectangle() : m_img(), m_thetaBins(), m_thetaMin(), m_thetaMax(), m_rhoBins(), m_theta_vec(){};

/*************************************************************************************/
HoughRectangle::HoughRectangle(int L_window, int thetaBins, int rhoBins, float thetaMin, float thetaMax) {
    m_thetaBins = thetaBins;
    m_thetaMin = thetaMin;
    m_thetaMax = thetaMax;
    m_rhoBins = rhoBins;
    m_L_window = L_window;

    m_theta_vec = VectorXf::LinSpaced(Sequential, thetaBins, thetaMin, thetaMax);

    m_rho_vec = LinearSpacedArray(-sqrt(pow(L_window / 2.0, 2) + pow(L_window / 2.0, 2)),
                                  sqrt(pow(L_window / 2.0, 2) + pow(L_window / 2.0, 2)), rhoBins);
}

/*************************************************************************************/
HoughRectangle::HoughRectangle(HoughRectangle::fMat& img, int thetaBins, int rhoBins, float thetaMin, float thetaMax) {
    m_img = img;
    m_thetaBins = thetaBins;
    m_thetaMin = thetaMin;
    m_thetaMax = thetaMax;
    m_rhoBins = rhoBins;

    m_theta_vec = VectorXf::LinSpaced(Sequential, thetaBins, thetaMin, thetaMax);

    m_rho_vec = LinearSpacedArray(-sqrt(pow(img.rows() / 2.0, 2) + pow(img.rows() / 2.0, 2)),
                                  sqrt(pow(img.rows() / 2.0, 2) + pow(img.rows() / 2.0, 2)), rhoBins);
}

/*************************************************************************************/
HoughRectangle::fMat HoughRectangle::ring(const HoughRectangle::fMat& img, const int& r_min, const int& r_max) {
    HoughRectangle::fMat result = img.replicate<1, 1>();
    float center_x, center_y;
    if (remainder(img.cols(), 2) != 0) {
        center_x = (img.cols() - 1) / 2;
    } else {
        center_x = (img.cols() / 2);
    }
    if (remainder(img.rows(), 2) != 0) {
        center_y = (img.rows() - 1) / 2;
    } else {
        center_y = (img.rows() - 1) / 2;
    }

    for (int i = 0; i < img.cols(); ++i) {
        for (int j = 0; j < img.rows(); ++j) {
            float dist = sqrt(pow(i - center_x, 2) + pow(j - center_y, 2));
            if (dist < r_min or dist > r_max) {
                result(j, i) = 0;
            }
        }
    }

    return result;
}

/*************************************************************************************/
HoughRectangle::fMat HoughRectangle::windowed_hough(const HoughRectangle::fMat& img, const int& r_min,
                                                    const int& r_max) {
    HoughRectangle::fMat ringed_subregion = ring(img, r_min, r_max);

    HoughRectangle::fMat wht = hough_transform(ringed_subregion);

    return wht;
}

/*************************************************************************************/
HoughRectangle::fMat HoughRectangle::apply_windowed_hough(const fMat& img, const int& L_window, const int& r_min,
                                                          const int& r_max) {
    // TODO FINISH
    for (int i = 0; i < img.rows() - L_window; ++i) {
        for (int j = 0; j < img.cols() - L_window; ++j) {
            // Applying circular mask to local region
            Matrix<float, Dynamic, Dynamic, RowMajor> subregion = img.block(i, j, L_window, L_window);
        }
    }
    return HoughRectangle::fMat{};
}

/*************************************************************************************/
void HoughRectangle::hough_transform(const fMat& img, fMat& acc) {
    // Cartesian coordinate vectors
    VectorXi vecX = VectorXi::LinSpaced(Sequential, img.cols(), 0, img.cols() - 1);
    VectorXi vecY = VectorXi::LinSpaced(Sequential, img.rows(), 0, img.rows() - 1);
    int mid_X = round(img.cols() / 2);
    int mid_Y = round(img.rows() / 2);
    vecX = vecX.array() - mid_X;
    vecY = vecY.array() - mid_Y;

    // Pre-compute cosines and sinuses:
    VectorXf cosT = cos(m_theta_vec.array() * PI / 180.0);
    VectorXf sinT = sin(m_theta_vec.array() * PI / 180.0);

    // Compute Hough transform
    std::vector<float>::iterator idx;
    int idx_rho;
    VectorXf rho_vec_tmp;
    for (int i = 0; i < img.rows(); ++i) {
        for (int j = 0; j < img.cols(); ++j) {
            if (img(i, j) != 0) {
                // generate sinusoidal curve
                rho_vec_tmp = vecX[j] * cosT + vecY[i] * sinT;

                // Find corresponding position and fill accumulator
                for (int k = 0; k < m_theta_vec.size(); ++k) {
                    idx = std::lower_bound(m_rho_vec.begin(), m_rho_vec.end(), rho_vec_tmp[k]);  // could be replaced
                    idx_rho = idx - m_rho_vec.begin() - 1;

                    // Fill accumulator
                    acc(idx_rho, k) = acc(idx_rho, k) + 1;
                }
            }
        }
    }
}
/*************************************************************************************/
HoughRectangle::fMat HoughRectangle::hough_transform(const fMat& img) {
    // Define accumulator matrix, theta and rho vectors
    HoughRectangle::fMat acc = MatrixXf::Zero(m_rhoBins, m_thetaBins);  // accumulator

    // Cartesian coordinate vectors
    VectorXi vecX = VectorXi::LinSpaced(Sequential, img.cols(), 0, img.cols() - 1);
    VectorXi vecY = VectorXi::LinSpaced(Sequential, img.rows(), 0, img.rows() - 1);
    int mid_X = round(img.cols() / 2);
    int mid_Y = round(img.rows() / 2);
    vecX = vecX.array() - mid_X;
    vecY = vecY.array() - mid_Y;

    // Pre-compute cosines and sinuses:
    VectorXf cosT = cos(m_theta_vec.array() * PI / 180.0);
    VectorXf sinT = sin(m_theta_vec.array() * PI / 180.0);

    // Compute Hough transform
    for (int i = 0; i < img.rows(); ++i) {
        for (int j = 0; j < img.cols(); ++j) {
            if (img(i, j) != 0) {
                // generate sinusoidal curve
                for (int k = 0; k < m_theta_vec.size(); ++k) {
                    // Calculate rho value
                    float rho_tmp = (vecX[j] * cosT[k] + vecY[i] * sinT[k]);

                    std::vector<float>::iterator idx;
                    idx = std::lower_bound(m_rho_vec.begin(), m_rho_vec.end(), rho_tmp);  // could be replaced
                    int idx_rho = idx - m_rho_vec.begin() - 1;

                    if (idx_rho < 0) {
                        idx_rho = 0;
                    }

                    // Fill accumulator
                    acc(idx_rho, k) = acc(idx_rho, k) + 1;
                    if (acc(idx_rho, k) > pow(2, 32)) {
                        std::cout << "Max value overpassed";
                    }
                }
            }
        }
    }

    return acc;
}


/*************************************************************************************/
HoughRectangle::fMat HoughRectangle::enhance_hough(const HoughRectangle::fMat& hough, const int& h, const int& w) {
    HoughRectangle::fMat houghpp = MatrixXf::Zero(hough.rows(), hough.cols());

    for (int i = h; i < hough.rows() - h; ++i) {
        for (int j = w; j < hough.cols() - w; ++j) {
            /*           double tmp =
             * pow(hough(i,j)/sqrt(hough.block(i-h/2,j-w/2,h,w).sum()),2);*/
            /*std::cout <<tmp<<" "<<hough(i,j)<<" "<<
             * hough.block(i-h/2,j-w/2,h,w).sum()<<std::endl;*/
            if (hough.block(i - h / 2, j - w / 2, h, w).sum() == 0) {
                houghpp(i, j) = 0;
            } else {
                houghpp(i, j) = pow(hough(i, j), 2) * h * w / hough.block(i - h / 2, j - w / 2, h, w).sum();
            }
        }
    }

    return houghpp;
}

/*************************************************************************************/
std::tuple<std::vector<float>, std::vector<float>> HoughRectangle::index_rho_theta(
    const std::vector<std::array<int, 2>>& indexes) {
    std::vector<float> rho_max(indexes.size());
    std::vector<float> theta_max(indexes.size());

    for (int i = 0; i < indexes.size(); ++i) {
        rho_max[i] = m_rho_vec[indexes[i][0]];
        theta_max[i] = m_theta_vec(indexes[i][1]);
    }

    return std::make_tuple(rho_max, theta_max);
}



