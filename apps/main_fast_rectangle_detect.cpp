#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Eigen/Dense>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <fstream>
#include "array"
#include "config.hpp"
#include "cxxopts.hpp"
#include "io.hpp"
#include "process_image.hpp"
#include "rectangle_utils.hpp"
#include "rectangle_detection.hpp"
#include "recursive_hough_transform.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"
#include "eigen_utils.hpp"


using Eigen::Dynamic;
using Eigen::Matrix;
using Eigen::RowMajor;

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

    // Init
    HoughRectangle ht(config.L_window, config.thetaBins, config.rhoBins, config.thetaMin, config.thetaMax);
    RecursiveHoughTransform fast_ht(config.L_window, config.thetaBins, config.rhoBins, config.thetaMin, config.thetaMax);

    // Loop over each pixel to find rectangle
    rectangles_T<int> rectangles;
    HoughRectangle::fMat acc(config.rhoBins, config.thetaBins);

    std::vector<int> acc_left_col;
    for (int i = 0; i < gray.rows() - config.L_window; ++i) {
        for (int j = 0; j < gray.cols() - config.L_window; ++j) {
            // First column pixel
            if (j == 0) {
                ht.hough_transform(gray.block(i, j, config.L_window, config.L_window), acc);
                continue;
            }
            
            // Hough transform
            std::array<Eigen::VectorXf, 2> columns = {gray.col(j), gray.col(j + config.L_window - 1)};
            std::vector<int> acc_column_previous;
            fast_ht.fast_hough_transform(acc, columns, acc_left_col);

            // Detect peaks
            std::vector<std::array<int, 2>> indexes = find_local_maximum(acc, config.min_side_length);
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

            std::array<float, 8> detected_rectangle = rectangle_detect::remove_duplicates(rectangles_tmp, 1, 4);
            auto rectangles_corners = convert_all_rects_2_corner_format(detected_rectangle, config.L_window, config.L_window);
            correct_offset_rectangle(rectangles_corners,j,i);

            // Concatenate
            rectangles.push_back(rectangles_corners);
        }
        std::cout << i << std::endl;
    }

    if (rectangles.size() == 0) {
        std::cout << "Did not detect any rectangle" << std::endl;
        exit(0);
    }

    // Clean up and save
     //std::array<int, 8> detected_rectangle = ht.remove_duplicates(rectangles, 1, 4);
     //auto rectangles_corners = convert_all_rects_2_corner_format(detected_rectangle, gray.rows(), gray.cols());
     eigen_io::save_rectangle(output_filename.c_str(), rectangles);

    return 0;
}
