#include "rectangle_utils.hpp"
#include <Eigen/Dense>
#include <array>
#include <cmath>
#include <vector>


//-----------------------------------------------------------------------------------------------------//
std::array<float, 3> convert_normal2cartesian(const float &angle, const float &rho) {
    std::array<float, 3> cartesian;

    cartesian[0] = cos((180 - angle) * M_PI / 180.0);
    cartesian[1] = sin((180 - angle) * M_PI / 180.0);
    cartesian[2] = -rho;

    return cartesian;
}
//-----------------------------------------------------------------------------------------------------//
void compute_line_intersections(const std::array<float, 3> &line1, const std::array<float, 3> &line2,
                                const std::array<float, 3> &line3, const std::array<float, 3> &line4,
                                rectangle_T<int> &polygon) {
    // What rounding does this do?
    polygon[0] = (-line1[2] * line3[1] + line1[1] * line3[2]) / (line1[0] * line3[1] - line1[1] * line3[0]);
    polygon[1] = (-line1[0] * line3[2] + line1[2] * line3[0]) / (line1[0] * line3[1] - line1[1] * line3[0]);

    polygon[2] = (-line1[2] * line4[1] + line1[1] * line4[2]) / (line1[0] * line4[1] - line1[1] * line4[0]);
    polygon[3] = (-line1[0] * line4[2] + line1[2] * line4[0]) / (line1[0] * line4[1] - line1[1] * line4[0]);

    polygon[4] = (-line2[2] * line3[1] + line2[1] * line3[2]) / (line2[0] * line3[1] - line2[1] * line3[0]);
    polygon[5] = (-line2[0] * line3[2] + line2[2] * line3[0]) / (line2[0] * line3[1] - line2[1] * line3[0]);

    polygon[6] = (-line2[2] * line4[1] + line2[1] * line4[2]) / (line2[0] * line4[1] - line2[1] * line4[0]);
    polygon[7] = (-line2[0] * line4[2] + line2[2] * line4[0]) / (line2[0] * line4[1] - line2[1] * line4[0]);
}

//-----------------------------------------------------------------------------------------------------//
void convert_cartesian2image_coordinates(rectangle_T<int> &rectangle, const int &x_size, const int &y_size) {
    int x_bias = int(x_size / 2);
    int y_bias = int(y_size / 2);

    rectangle[0] = rectangle[0] + x_bias;
    rectangle[1] = -rectangle[1] + y_bias;
    rectangle[2] = rectangle[2] + x_bias;
    rectangle[3] = -rectangle[3] + y_bias;
    rectangle[4] = rectangle[4] + x_bias;
    rectangle[5] = -rectangle[5] + y_bias;
    rectangle[6] = rectangle[6] + x_bias;
    rectangle[7] = -rectangle[7] + y_bias;
}
//-----------------------------------------------------------------------------------------------------//
void correct_offset_rectangle(rectangle_T<int> &rectangle,int x_bias, int y_bias) {

    rectangle[0] = rectangle[0] + x_bias; 
    rectangle[1] = rectangle[1] + y_bias; 
    rectangle[2] = rectangle[2] + x_bias; 
    rectangle[3] = rectangle[3] + y_bias; 
    rectangle[4] = rectangle[4] + x_bias; 
    rectangle[5] = rectangle[5] + y_bias; 
    rectangle[6] = rectangle[6] + x_bias; 
    rectangle[7] = rectangle[7] + y_bias; 

}
//-----------------------------------------------------------------------------------------------------//
rectangle_T<int> convert_normal_rect2_corners_rect(const std::array<float, 8> &in_rectangle, const int &x_size,
                                                     const int &y_size) {
    rectangle_T<int> rectangle;

    // Angle shift between first set of lines and second set of lines
    float bias;
    if (in_rectangle[0] < 0)
        bias = 90;
    else
        bias = -90;

    // Create rectangle lines
    std::array<float, 3> line1 = convert_normal2cartesian(in_rectangle[0], in_rectangle[1]);
    std::array<float, 3> line2 = convert_normal2cartesian(in_rectangle[0], -in_rectangle[1]);
    std::array<float, 3> line3 = convert_normal2cartesian(in_rectangle[0] + bias, in_rectangle[2]);
    std::array<float, 3> line4 = convert_normal2cartesian(in_rectangle[0] + bias, -in_rectangle[2]);

    // Compute rectangle corners
    compute_line_intersections(line1, line2, line3, line4, rectangle);

    // Convert to image format
    convert_cartesian2image_coordinates(rectangle, x_size, y_size);
    
    return rectangle;
}

//-----------------------------------------------------------------------------------------------------//
rectangles_T<int> convert_all_rects_2_corner_format(const rectangles_T<float>  &rectangles,
                                                                  const int &x_size, const int &y_size) {
    rectangles_T<int> rectangles_cart;
    for (std::array<float, 8> rect : rectangles)
        rectangles_cart.push_back(convert_normal_rect2_corners_rect(rect, x_size, y_size));

    return rectangles_cart;
}

//-----------------------------------------------------------------------------------------------------//
rectangle_T<int> convert_all_rects_2_corner_format(const rectangle_T<float> &rectangles, const int &x_size,
                                                     const int &y_size) {
    rectangle_T<int> rectangles_cart = convert_normal_rect2_corners_rect(rectangles, x_size, y_size);

    return rectangles_cart;
}
