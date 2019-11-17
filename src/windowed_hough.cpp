#include <iostream>
#include "cxxopts.hpp"
#include "Eigen/Dense"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "io.hpp"
#include "process_image.hpp"

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

    int r_min = 21;
    int r_max = 81;
    int thetaBins = 256;
    int thetaMin = 0;
    int thetaMax = 90;
    int rhoBins = 256;

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
    Eigen::MatrixXf wht = ht.windowed_hough(gray,r_min,r_max,thetaBins,rhoBins,thetaMin,thetaMax); 

    save_image(wht,output_path,thetaBins*rhoBins,thetaBins,rhoBins);

}
