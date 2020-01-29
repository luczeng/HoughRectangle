#include <array>
#include <vector>

 namespace rectangle_detect {
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
     * @param[out] rectangles a vector of arrays of size 8. First element is the angle, 2nd corresponding rho, 3rd rho
     * of opposite corner. Next 5 elements are respectively: error on rho for opposite corners, error on theta for
     * opposite corners and error on perpendicularity.
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
    std::array<int, 8> remove_duplicates(std::vector<std::array<int, 8>> rectangles, float a, float b);
    std::array<float, 8> remove_duplicates(std::vector<std::array<float, 8>> rectangles, float a, float b);
};
