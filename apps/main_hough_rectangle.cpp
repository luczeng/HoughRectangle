#include <iostream>
// TODO(luczeng): It's always a bit hacky to make #include depend on specific #define statements.
//                Better encapsulate in a seperate header
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Eigen/Dense>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <fstream>
#include "config.hpp"
#include "cxxopts.hpp"
#include "io.hpp"
#include "process_image.hpp"
#include "rectangle_utils.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"
#include "rectangle_detection.hpp"


int main(int argc, char* argv[]) {
    // Nota bene: casting big images to unsigned char in Eigen result in a
    // segmentation fault on my machine for some unknown reasons. Compiler
    // complains that the array is too big. We have therefore chosen the
    // following way to convert Eigen matrix to unsigned char *

    // Parse arguments
    cxxopts::Options options("Runs Hough rectangle detection algorithm");
    options.add_options()("i,image_path", "Path to binary (0-255) input image", cxxopts::value<std::string>())(
        "o,output_path", "Path to .txt file where detected rectangles will be saved", cxxopts::value<std::string>());
    auto result = options.parse(argc, argv);

    std::string filename = result["image_path"].as<std::string>();
    std::string output_filename = result["output_path"].as<std::string>();

    // Parse config file
    Config config;
    std::ifstream is("../src/configs.json");
    cereal::JSONInputArchive archive(is);
    archive(config);

    // Load image and prepare matrix
    Matrix<float, Dynamic, Dynamic, RowMajor> gray = eigen_io::read_image(filename.c_str());

    // Perform Hough transform
    HoughRectangle ht(config.L_window, config.thetaBins, config.rhoBins, config.thetaMin, config.thetaMax);

    // Loop over each pixel to find rectangle
    rectangles_T<int> rectangles;
    HoughRectangle::fMat hough_img(config.rhoBins, config.thetaBins);

    for (int i = 0; i < gray.rows() - config.L_window; ++i) {
        std::cout << "Row " << i << "/" << gray.rows() << std::endl;
        for (int j = 0; j < gray.cols() - config.L_window; ++j) {
            // Hough transform
            hough_img.setZero();
            ht.hough_transform(gray.block(i, j, config.L_window, config.L_window), hough_img);

            // Detect peaks
            std::vector<std::array<int, 2>> indexes = find_local_maximum(hough_img, config.min_side_length);
            std::vector<float> rho_maxs, theta_maxs;
            std::tie(rho_maxs, theta_maxs) = ht.index_rho_theta(indexes);

            // Find pairs
            std::vector<std::array<float, 4>> pairs =
                rectangle_detect::find_pairs(rho_maxs, theta_maxs, config.T_rho, config.T_theta, config.T_l);
            if (pairs.size() == 0) {
                continue;
            }  // no pairs detected

            // Find rectangle
            rectangles_T<float> rectangles_tmp = rectangle_detect::match_pairs_into_rectangle(pairs, config.T_alpha);
            if (rectangles_tmp.size() == 0) {
                continue;
            }  // if no rectangle detected
            else {
                std::cout << "Rectangle detected"<<" "<< i << " "<< j <<std::endl;
            }
            std::array<float, 8> detected_rectangle = rectangle_detect::remove_duplicates(rectangles_tmp, 1, 4);
            auto rectangles_corners = convert_all_rects_2_corner_format(detected_rectangle, config.L_window, config.L_window);
            correct_offset_rectangle(rectangles_corners,j,i);

            // Concatenate
            rectangles.push_back(rectangles_corners);
        }
    }

    if (rectangles.size() == 0) {
        std::cout<< "Did not detect any rectangle" <<std::endl;
        exit(0);
    }

    // Clean up and save
    //std::array<int, 8> detected_rectangle = rectangle_detect::remove_duplicates(rectangles, 1, 4);
    //auto rectangles_corners = convert_all_rects_2_corner_format(detected_rectangle, gray.rows(), gray.cols());
    eigen_io::save_rectangle(output_filename.c_str(), rectangles);

    return 0;
}
