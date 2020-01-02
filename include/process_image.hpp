#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H
#include <Eigen/Dense>
#include <array>
#include <tuple>
#include "config.hpp"

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

/**
 *
 * Class containing all the tools to perform rectangle detection on an image.
 *
 * This class implements the Hough transform, enhanced hough transform, windowed hough transform and utility functions
 * in order to compute the so called Hough Rectangle detection.
 *
 */
class HoughRectangle {
   private:
    int m_thetaBins;
    int m_thetaMin;
    int m_thetaMax;
    int m_rhoBins;

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
    HoughRectangle(fMat &img, int thetaBins = 256, int rhoBins = 256, float thetaMin = -90,
                   float thetaMax = 90);  // declaration

   public:
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
     * \param[out] acc accumulator (windowed hough transform)
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

    /*
     * Match detected peaks into pairs
     *
     * @param[in] rho_maxs vector specifying rho positions of detected peaks
     * @param[in] theta_maxs vector specifying theta positions of detected peaks
     * @param[out] pairs vector containing the extended peaks of pairs. 1st element = rho, 2nd = theta, 3rd = error on
     * rho, 4th = error on theta
     */
    std::vector<std::array<float, 4>> find_pairs(const std::vector<float> &rho_maxs,
                                                 const std::vector<float> &theta_maxs, const float &T_rho,
                                                 const float &T_t, const float &T_L);

    /**
     * Matches detected peaks into rectangle
     *
     * @param[in] rho_maxs vector specifying rho positions of detected peaks
     * @param[in] theta_maxs vector specifying theta positions of detected peaks
     * @param[out] rectangles a vector of arrays of size 3. First element is the angle, 2nd corresponding rho, 3rd rho
     * of opposite corner.
     */
    std::vector<std::array<float, 8>> match_pairs_into_rectangle(const std::vector<std::array<float, 4>> &pairs,
                                                                 const float &T_alpha);

    /**
     * Filters rectangles based on criteria that evalutes the parallelism, orthogonality and symmetry of the rectangles.
     * Returns the best rectangle
     *
     * @param[in] rectangles rectangles to filter
     * @param[in] a weight coefficient for degrees. A good value is a = 1
     * @param[in] b weight coefficient for pixels. A good value is b = 4
     * @param[out] rect best rectangle found based on criteria
     */
    std::array<float, 8> remove_duplicates(std::vector<std::array<float, 8>> rectangles, float a,
                                                           float b);
};

#endif
