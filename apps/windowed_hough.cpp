#include <iostream>
#include "Eigen/Dense"
#include "cxxopts.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <fstream>
#include "config.hpp"
#include "io.hpp"
#include "process_image.hpp"
#include "stb_image.h"
#include "stb_image_write.h"

using namespace Eigen;

int main(int argc, char* argv[]) {
    // Parse arguments
    cxxopts::Options options("Runs windowed hough transfrom on an edge image");
    options.add_options()("i,image_path", "Path to input image", cxxopts::value<std::string>())(
        "o,out_img_path", "Path to save image (png)", cxxopts::value<std::string>());
    auto parsing_results = options.parse(argc, argv);

    std::string input_path = parsing_results["image_path"].as<std::string>();
    std::string output_path = parsing_results["out_img_path"].as<std::string>();

    // Parse config file
    Config config;
    std::ifstream is("../src/configs.json");
    cereal::JSONInputArchive archive(is);
    archive(config);

    // Load image and prepare matrix
    Matrix<float, Dynamic, Dynamic, RowMajor> gray = eigen_io::read_image(input_path.c_str());

    // Process image
    HoughRectangle ht(gray, config.thetaBins, config.rhoBins, config.thetaMin, config.thetaMax);
    Matrix<float, Dynamic, Dynamic, RowMajor> wht = ht.windowed_hough(gray, config.r_min, config.r_max);

    eigen_io::save_image(wht, output_path, config.thetaBins * config.rhoBins, config.thetaBins, config.rhoBins);
}
#endif
#endif
