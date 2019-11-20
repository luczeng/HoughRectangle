#include <iostream>
#include "cxxopts.hpp"
#include "Eigen/Dense"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "io.hpp"
#include "process_image.hpp"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include "config.hpp"
#include <fstream>

using namespace Eigen;

int main(int argc, char * argv[]){
    ////////////////////////////////////////////////////////////////////////
    // Parse arguments
    ////////////////////////////////////////////////////////////////////////
    cxxopts::Options options("Runs windowed hough transfrom on an edge image");
    options.add_options()("i,image_path","Path to input image",cxxopts::value<std::string>())
        ("o,out_img_path","Path to save image (png)",cxxopts::value<std::string>());
    auto parsing_results = options.parse(argc,argv);

    std::string input_path = parsing_results["image_path"].as<std::string>();
    std::string output_path = parsing_results["out_img_path"].as<std::string>();

    // Parse config file
    Config config;
    std::ifstream is("../src/configs.json");
    cereal::JSONInputArchive archive(is);
    archive(config);

    int thetaBins = 512;
    int thetaMin = -90;
    int thetaMax = 90;
    int rhoBins = 512;
    int h =32;

    ////////////////////////////////////////////////////////////////////////
    // Load image and prepare matrix
    ////////////////////////////////////////////////////////////////////////
    int x, y, n;
    std::unique_ptr<unsigned char[]> data(
        stbi_load(input_path.c_str(), &x, &y, &n, 0));
    if (data == NULL) {
        std::cout << "Cant load image" << std::endl;
    }

    // Port array to Eigen matrix
    MatrixXf gray;
    convert_RawBuff2Mat(data, gray, x, y);

    ////////////////////////////////////////////////////////////////////////
    // Process image
    ////////////////////////////////////////////////////////////////////////
    HoughRectangle ht(gray);
    Eigen::MatrixXf wht = ht.hough_transform(gray,config.thetaBins,config.rhoBins,config.thetaMin,config.thetaMax); 

    save_image(wht,output_path,config.thetaBins*config.rhoBins,config.thetaBins,config.rhoBins);

}
