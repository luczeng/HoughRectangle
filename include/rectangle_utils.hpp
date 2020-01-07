#include <vector>
/**
 * Convert normal coordinate to cartesian coordinates
 *
 * @param[in] angle normal
 * @param[in] rho rho
 * @param[out] cartesian ax + by + c = 0, a,b,c
 */
std::array<float, 3> convert_normal2cartesian(const float &angle, const float &rho);

/**
 * Computes intersection of 4 lines. 2 first lines should not be parrallel to 2 second lines. Corners are ordered as
 * follow: c1 = intersect line1 & line3, c2 =intersect line1 & line4, c3 = intersect line2&line3, c4 = intersect line2 &
 * line4
 *
 * @param[in] line1 line in cartesian format a,b,c, ax + by + c = 0
 * @param[in] line1 line in cartesian format a,b,c, ax + by + c = 0
 * @param[in] line1 line in cartesian format a,b,c, ax + by + c = 0
 * @param[in] line1 line in cartesian format a,b,c, ax + by + c = 0
 * @param[in,out] polygon output polygon in format x1,y1,x2,y2,x3,y3,..
 */
void compute_line_intersections(const std::array<float, 3> &line1, const std::array<float, 3> &line2,
                                const std::array<float, 3> &line3, const std::array<float, 3> &line4,
                                std::array<int, 8> &polygon);

/**
 * Converts cartesian coordinates in image coordinates. Require to know the size of the image. For now, converts
 * directly to int by rounding to the upper integer.
 *
 * @param[in,out] rectangle rectangle in x1,y1,x2,y2 format
 * @param[in] x_size number of column of image
 * @param[in] y_size number of rows of image
 */
void convert_cartesian2image_coordinates(std::array<int, 8> &rectangle, const int &x_size, const int &y_size);

/**
 * Convert normal rectangle into corner rectangle and image coordinate
 *
 * @param[in,out] rectangle rectangle in cartesian coordinates and corner format
 * @param[in] x_size number of columns of image
 * @param[in] y_size number of rows of image
 */
std::array<int, 8> convert_normal_rect2_corners_rect(const std::array<float, 8> &in_rectangle, const int &x_size,
                                                     const int &y_size);

/**
 * Convert all rectangles to corner format and image coordinate
 *
 * @param[in,out] rectangle rectangle to convert
 * @param[in] x_size number of columns of image
 * @param[in] y_size number of rows of image
 */
std::vector<std::array<int, 8>> convert_all_rects_2_cartesian(const std::vector<std::array<float, 8>> &rectangle,
                                                              const int &x_size, const int &y_size);

/**
 * Convert rectangle to corner format
 *
 * @param[in,out] rectangle rectangle to convert
 * @param[in] x_size number of columns of image
 * @param[in] y_size number of rows of image
 */
std::array<int, 8> convert_all_rects_2_corner_format(const std::array<float, 8> &rectangle, const int &x_bias,
                                                     const int &y_bias);
