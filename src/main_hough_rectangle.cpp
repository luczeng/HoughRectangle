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
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"

using namespace Eigen;
// void convertMat2UC(MatrixXf gray,unsigned char *& gray_UC,int size);

int main(int argc, char* argv[]) {
    // Nota bene: casting big images to unsigned char in Eigen result in a
    // segmentation fault on my machine for some unknown reasons. Compiler
    // complains that the array is too big. We have therefore chosen the
    // following way to convert Eigen matrix to unsigned char *

    ////////////////////////////////////////////////////////////////////////
    // Parse arguments
    ////////////////////////////////////////////////////////////////////////
    cxxopts::Options options("Runs Hough rectangle detection algorithm");

    options.add_options()("i,image_path", "Path to binary input image",
                          cxxopts::value<std::string>())(
        "o,output_path", "Path to save result", cxxopts::value<std::string>());
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
    int x, y, n;
    std::unique_ptr<unsigned char[]> data(
        stbi_load(filename.c_str(), &x, &y, &n, 0));
    if (data == NULL) {
        std::cout << "Cant load image" << std::endl;
    }

    // Port array to Eigen matrix
    MatrixXf gray;
    convert_RawBuff2Mat(data, gray, x, y);

    ////////////////////////////////////////////////////////////////////////
    // Perform Hough transform
    ////////////////////////////////////////////////////////////////////////
    HoughRectangle ht(gray);
    ht.windowed_hough(gray,config.L_window,config.r_min,config.r_max,config.thetaBins,config.rhoBins,config.thetaMin,config.thetaMax);
    //ht.hough_transform(gray, config);

    return 0;
}
