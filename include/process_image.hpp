#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H
#include <Eigen/Dense>
#include <array>
#include <tuple>
#include "config.hpp"


/**
 *
 * Class containing all classic Hough rectangle methods
 *
 * This class implements the Hough transform, enhanced hough transform, windowed hough transform 
 */
class HoughRectangle {
   protected:
    int m_thetaBins;
    int m_thetaMin;
    int m_thetaMax;
    int m_rhoBins;
    int m_L_window;

    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> m_img;

   public:
    Eigen::VectorXf m_theta_vec;  // make those private later with getters
    std::vector<float> m_rho_vec;
    /*
     * Rectangle class constructor
     *
     * @param img Eigen float, Dynamic, RowMajor matrix to process
     */
    typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> fMat;
    HoughRectangle();
    HoughRectangle(int L_window, int thetaBins, int rhoBins, float thetaMin, float thetaMax);
    HoughRectangle(fMat &img, int thetaBins = 256, int rhoBins = 256, float thetaMin = -90,
                   float thetaMax = 90);  // declaration

   public:
    /**
     * Applies the classic Hough transform on the image
     *
     * @param[in] img input image to be processed
     * @param[in, out] acc accumulator (hough transform). Size must be rho_vec_size x theta_vec_size.
     */
    void hough_transform(const fMat &img, fMat &acc);

    /**
     * Applies the classic Hough transform on the image
     *
     * @param[in] img input image to be processed
     * @param[out] acc accumulator (hough transform)
     */
    fMat hough_transform(const fMat &img);

    /**
     * Performs the Windowed hough transform on a single patch
     *
     * \param[in] img input float image
     * \param[out] acc accumulator (windowed hough transform). Y-axis: rho. X-axis: theta.
     */
    fMat windowed_hough(const fMat &img, const int &r_min, const int &r_max);

    /**
     * Applies the Windowed hough transform on the whole image
     *
     * @param[in] img image to process
     * @param[in] L_window size of the window on which the Hough transform is applied to. Should be > rectangle length
     * @param[in] r_min inner size of the ring. Should be smaller than the rectangle
     * @param[in] r_max outer size of the ring. Should be bigger than the rectangle
     * @param[out]
     */
    fMat apply_windowed_hough(const fMat &img, const int &L_window, const int &r_min, const int &r_max);

    /**
     * Computes enhanced Hough transform by amplying high pixels
     *
     * @param[in] hough transform
     * @param[in] h height of patch
     * @param[in] w width of patch
     * @param[out] houghpp enhanced hough transform
     */
    fMat enhance_hough(const fMat &hough, const int &h, const int &w);

    /**
     * Applies a ring on the input matrix by putting to zero all pixels outside the ring
     *
     * @param[in] img Eigen matrix
     * @param[in] r_min radius of inner ring
     * @param[in] r_max radius of outer ring
     * @param[out] result ringed output matrix
     */
    fMat ring(const fMat &img, const int &r_min, const int &r_max);

    /**
     * Returns vectors of theta and rho positions corresponding to the input indexes
     *
     * @param[in] indexes vector of array containing index for rho in 1st element and theta in 2nd
     * @param[out] rho_vec, theta_vec tuple of vectors containing the rho and theta maximums
     */
    std::tuple<std::vector<float>, std::vector<float>> index_rho_theta(const std::vector<std::array<int, 2>> &indexes);
};

#endif
