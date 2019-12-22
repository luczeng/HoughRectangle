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
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"

// TODO(luczeng): better to use separate using clauses, otherwise you quickly get name clashes.
using Eigen::Matrix;
using Eigen::Dynamic;
using Eigen::RowMajor;

int main(int argc, char* argv[]) {
    // Nota bene: casting big images to unsigned char in Eigen result in a
    // segmentation fault on my machine for some unknown reasons. Compiler
    // complains that the array is too big. We have therefore chosen the
    // following way to convert Eigen matrix to unsigned char *


    // TODO(luczeng): personal preference, but these type of comments scare me :p just a oneliner should be enough
    ////////////////////////////////////////////////////////////////////////
    // Parse arguments
    ////////////////////////////////////////////////////////////////////////
    cxxopts::Options options("Runs Hough rectangle detection algorithm");
    options.add_options()("i,image_path", "Path to binary (0-255) input image",
                          cxxopts::value<std::string>())(
        "o,output_path", "Path to .txt file where detected rectangles will be saved", cxxopts::value<std::string>());
    auto result = options.parse(argc, argv);

    std::string filename = result["image_path"].as<std::string>();
    std::string output_filename = result["output_path"].as<std::string>();

    // Parse config file
    Config config;
    std::ifstream is("../src/configs.json");
    cereal::JSONInputArchive archive(is);
    archive(config);

    ////////////////////////////////////////////////////////////////////////
    // Load image and prepare matrix
    ////////////////////////////////////////////////////////////////////////
    Matrix<float,Dynamic,Dynamic,RowMajor> gray = read_image(filename.c_str());

    ////////////////////////////////////////////////////////////////////////
    // Perform Hough transform
    ////////////////////////////////////////////////////////////////////////
    HoughRectangle ht(gray,config.thetaBins,config.rhoBins,config.thetaMin,config.thetaMax);
    HoughRectangle::fMat hough_img = ht.hough_transform(gray);

    // Detect peaks
    std::vector<std::array<int, 2>> indexes = find_local_maximum(hough_img, config.min_side_length);
    std::vector<float> rho_maxs, theta_maxs;
    std::tie(rho_maxs, theta_maxs) = ht.index_rho_theta(indexes);

    // Find pairs
    std::vector<std::array<float,4>> pairs = ht.find_pairs(rho_maxs,theta_maxs,config.T_rho,config.T_theta,config.T_l);
    
    // Find rectangle
    std::vector<std::array<float, 3>> rectangles = ht.match_pairs_into_rectangle(pairs,config.T_alpha);
    std::vector<std::array<int,8>> rectangles_corners = convert_all_rects_2_cartesian(rectangles,gray.rows()/2,gray.cols()/2);
    std::cout << "Found "<<rectangles_corners.size()<<" rectangles"<<std::endl;
    save_rectangle("rectangles.txt",rectangles_corners);

    return 0;
}
