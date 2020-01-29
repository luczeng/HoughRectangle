#include <iostream>
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
#include "rectangle_detection.hpp"
#include "rectangle_utils.hpp"
#include "eigen_utils.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"


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

    // Perform Hough transform
    HoughRectangle ht(gray, config.thetaBins, config.rhoBins, config.thetaMin, config.thetaMax);
    HoughRectangle::fMat hough_img = ht.hough_transform(gray);

    // Detect peaks
    std::vector<std::array<int, 2>> indexes = find_local_maximum(hough_img, config.min_side_length);
    std::vector<float> rho_maxs, theta_maxs;
    std::tie(rho_maxs, theta_maxs) = ht.index_rho_theta(indexes);

    // Find pairs
    std::vector<std::array<float, 4>> pairs =
        rectangle_detect::find_pairs(rho_maxs, theta_maxs, config.T_rho, config.T_theta, config.T_l);

    // Find rectangle
    std::vector<std::array<float, 8>> rectangles = rectangle_detect::match_pairs_into_rectangle(pairs, config.T_alpha);
    std::array<float, 8> detected_rectangle = rectangle_detect::remove_duplicates(rectangles, 1, 4);

    // Cartesian rectangles
    auto rectangles_corners = convert_all_rects_2_corner_format(detected_rectangle, gray.cols(), gray.rows());

    eigen_io::save_rectangle(output_filename, rectangles_corners);

    return 0;
}
