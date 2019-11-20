#include "process_image.hpp"
#include <math.h>
#include <stdlib.h>
#include <Eigen/Dense>
#include <iostream>
#include "config.hpp"
#include "io.hpp"
#include "stb_image.h"
#include "stb_image_write.h"

#define PI 3.14159265

using namespace Eigen;

void Log(const char* message) { std::cout << message << std::endl; }

/*
 * Function to make sure binary is 0 and 255
 */
void normalise_img(MatrixXf& img) {

    MatrixXf high;
    high.setOnes(img.rows(), img.cols());
    high *= 255;

    MatrixXf low;
    low.setZero(img.rows(), img.cols());

    MatrixXf tmp = (img.array() > 128.0).select(high, low);

    img = tmp;
}

/*
 * Returns a linearly spaced array
 */
std::vector<float> LinearSpacedArray(float a, float b, std::size_t N)
{
    double h = (b - a) / static_cast<float>(N - 1);
    std::vector<float> xs(N);
    std::vector<float>::iterator x;
    float val;
    for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h) {
        *x = val;
    }
    return xs;
}

// Rectangle class constructor
HoughRectangle::HoughRectangle(MatrixXf& img) { m_img = img; }

// Applies a ring on the input matrix
MatrixXf HoughRectangle::ring(MatrixXf& img, int r_min, int r_max) {
    MatrixXf result = img.replicate<1, 1>();
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

// Performs the Windowed hough transform
MatrixXf HoughRectangle::windowed_hough(MatrixXf& img, int r_min, int r_max,
                                        int thetaBins, int rhoBins,
                                        float thetaMin, float thetaMax) {
    MatrixXf ringed_subregion = ring(img, r_min, r_max);
    Eigen::MatrixXf wht = hough_transform(ringed_subregion, thetaBins, rhoBins,
                                          thetaMin, thetaMax);

    return wht;
}

// Applies the Windowed hough transform on the whole image
MatrixXf HoughRectangle::apply_windowed_hough(MatrixXf& img, int L_window,
                                              int r_min, int r_max,
                                              int thetaBins, int rhoBins,
                                              float thetaMin, float thetaMax) {
    for (int i = 0; i < img.rows() - L_window; ++i) {
        for (int j = 0; j < img.cols() - L_window; ++j) {
            // Applying circular mask to local region
            MatrixXf subregion = img.block(i, j, L_window, L_window);
        }
    }
}

// Applies the classic Hough transform
MatrixXf HoughRectangle::hough_transform(MatrixXf& img, int thetaBins,
                                         int rhoBins, float thetaMin,
                                         float thetaMax) {
    // Define accumulator matrix, theta and rho vectors
    MatrixXf acc = MatrixXf::Zero(rhoBins,thetaBins);  // accumulator
    VectorXf theta =
        VectorXf::LinSpaced(Sequential, thetaBins, thetaMin, thetaMax);
    std::vector<float> rho = LinearSpacedArray(
        -sqrt(pow(img.rows() / 2.0, 2) + pow(img.rows() / 2.0, 2)), sqrt(pow(img.rows() / 2.0, 2) + pow(img.rows() / 2.0, 2)),
        rhoBins);

    // Cartesian coordinate vectors
    VectorXi vecX =
        VectorXi::LinSpaced(Sequential, img.cols(), 0, img.cols() - 1);
    VectorXi vecY =
        VectorXi::LinSpaced(Sequential, img.rows(), 0, img.rows() - 1);
    int mid_X = round(img.cols() / 2);
    int mid_Y = round(img.rows() / 2);
    vecX = vecX.array() - mid_X;
    vecY = vecY.array() - mid_Y;

    // Pre-compute cosines and sinuses:
    VectorXf cosT = cos(theta.array() * PI / 180.0);
    VectorXf sinT = sin(theta.array() * PI / 180.0);

    // Compute Hough transform
    for (int i = 0; i < img.rows(); ++i) {
        for (int j = 0; j < img.cols(); ++j) {
            if (img(i, j) != 0) {
                // generate sinusoidal curve
                for (int k = 0; k < theta.size(); ++k) {
                    // Calculate rho value
                    float rho_tmp = (vecX[j] * cosT[k] + vecY[i] * sinT[k]);

                    std::vector<float>::iterator idx;
                    idx = std::lower_bound(rho.begin(), rho.end(), rho_tmp);
                    int idx_rho = idx - rho.begin() - 1;
                    // std::cout <<rho_tmp<<std::endl;
                    if (idx_rho < 0) {
                        idx_rho = 0;
                    }

                    // Fill accumulator
                    acc(idx_rho,k) = acc(idx_rho,k) + 1;
                    if (acc(idx_rho,k) > pow(2, 32)) {
                        std::cout << "Max value overpassed";
                    }
                }
            }
        }
    }

    // Enhanced HT

    return acc;
}

/*
* Computes enhanced Hough transform
*/
MatrixXf HoughRectangle::enhance_hough(MatrixXf& hough,int h, int w){

    MatrixXf houghpp = MatrixXf::Zero(hough.rows(),hough.cols());

    for (int i = h; i < hough.rows() - h; ++i) {
        for (int j = w; j < hough.cols() - w; ++j) {
            /*           double tmp =
             * pow(hough(i,j)/sqrt(hough.block(i-h/2,j-w/2,h,w).sum()),2);*/
            /*std::cout <<tmp<<" "<<hough(i,j)<<" "<<
             * hough.block(i-h/2,j-w/2,h,w).sum()<<std::endl;*/
            if (hough.block(i - h / 2, j - w / 2, h, w).sum() == 0) {
                houghpp(i, j) = 0;
            } else {
                houghpp(i, j) = pow(hough(i, j), 2) * h * w /
                                hough.block(i - h / 2, j - w / 2, h, w).sum();
            }
        }
    }

    return houghpp;

}
