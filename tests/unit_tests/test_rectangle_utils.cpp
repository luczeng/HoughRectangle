#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "io.hpp"
#include "rectangle_utils.hpp"
#include "string"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <array>
#include "stb_image.h"
#include "stb_image_write.h"

TEST_CASE("Tests the rectangle utils") {
    SECTION("Convert normal line to cartesian coordinates") {
        // Vertical line
        std::array<float, 3> vert_line = convert_normal2cartesian(0, 20);
        std::array<float, 3> vert_line_gt = {-1, 0, -20};

        REQUIRE(vert_line[0] == Approx(vert_line_gt[0]));
        REQUIRE(vert_line[1] == Approx(vert_line_gt[1]).margin(0.00001));
        REQUIRE(vert_line[2] == Approx(vert_line_gt[2]));

        // Horizontal line
        std::array<float, 3> horiz_line = convert_normal2cartesian(90, 20);
        std::array<float, 3> horiz_line_gt = {0, 1, -20};

        REQUIRE(horiz_line[0] == Approx(horiz_line_gt[0]).margin(0.00001));
        REQUIRE(horiz_line[1] == horiz_line_gt[1]);
        REQUIRE(horiz_line[2] == horiz_line_gt[2]);

        // 45° line
        std::array<float, 3> line = convert_normal2cartesian(135, 0);
        std::array<float, 3> line_gt = {float(1 / sqrt(2)), float(1 / sqrt(2)), 0};

        REQUIRE(line[0] == line_gt[0]);
        REQUIRE(line[1] == line_gt[1]);
        REQUIRE(line[2] == line_gt[2]);
    }

    SECTION("Computes line intersection") {
        // Horitontal rectangle
        std::array<float, 3> line1 = {1, 0, -1};
        std::array<float, 3> line2 = {1, 0, 1};
        std::array<float, 3> line3 = {0, 1, -1};
        std::array<float, 3> line4 = {0, 1, 1};

        std::array<int, 8> rectangle;
        std::array<int, 8> rectangle_gt = {1, 1, 1, -1, -1, 1, -1, -1};

        compute_line_intersections(line1, line2, line3, line4, rectangle);

        REQUIRE(rectangle == rectangle_gt);

        // Rotated rectangle
        std::array<float, 3> line5 = {1, 1, -1};
        std::array<float, 3> line6 = {1, 1, 1};
        std::array<float, 3> line7 = {-1, 1, -1};
        std::array<float, 3> line8 = {-1, 1, 1};

        std::array<int, 8> rectangle2;
        std::array<int, 8> rectangle2_gt = {0, 1, 1, 0, -1, 0, 0, -1};

        compute_line_intersections(line5, line6, line7, line8, rectangle2);
        REQUIRE(rectangle2 == rectangle2_gt);
    }

    SECTION("Convert rectangle to image coordinates") {
        // Horizontal rectangle
        std::array<int, 8> rectangle = {1, 1, 1, -1, -1, 1, -1, -1};
        int x_size = 64;
        int y_size = 128;
        std::array<int, 8> rectangle_gt = {33, 63, 33, 65, 31, 63, 31, 65};
        convert_cartesian2image_coordinates(rectangle, x_size, y_size);

        REQUIRE(rectangle == rectangle_gt);
    }

    SECTION("Correct rectangle position given offset of window from which it was computed")
    {
        std::array<int, 8> rectangle = {33, 63, 33, 65, 31, 63, 31, 65};
        std::array<int, 8> rectangle_gt = {43, 83, 43, 85, 41, 83, 41, 85};

        correct_offset_rectangle(rectangle,10,20);

        REQUIRE( rectangle == rectangle_gt);
    }

    SECTION("Convert rectangle to corners") {
        // Conversion for "0,0" image
        std::array<float, 8> rectangle_normal = {0, 10, 10, 0, 0, 0, 0, 0};

        std::array<int, 8> rectangle = convert_normal_rect2_corners_rect(rectangle_normal, 0, 0);
        std::array<int, 8> rectangle_gt = {-10, 10, -10, -10, 10, 10, 10, -10};

        REQUIRE(rectangle_gt == rectangle);

        // Conversion for normal size image
        std::array<float, 8> rectangle_normal2 = {0, 10, 10, 0, 0, 0, 0, 0};
        int size_x = 64;
        int size_y = 128;

        std::array<int, 8> rectangle2 = convert_normal_rect2_corners_rect(rectangle_normal2, 64, 128);
        std::array<int, 8> rectangle_gt2 = {22, 74, 22, 54, 42, 74, 42, 54};

        REQUIRE(rectangle_gt2 == rectangle2);
    }
}

#endif
#endif
