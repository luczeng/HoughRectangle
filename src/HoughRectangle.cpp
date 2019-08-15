#include <iostream> 
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <Eigen/Dense>
#include "string"
#include "cxxopts.hpp"
#include "io.hpp"
#include "process_image.hpp"
#include <cereal/cereal.hpp>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include "config.hpp"

using namespace Eigen;
//void convertMat2UC(MatrixXf gray,unsigned char *& gray_UC,int size);

int main(int argc, char* argv[] ){
	//Nota bene: casting big images to unsigned char in Eigen result in a segmentation fault. Compiler complains that the array is too big. We have therefore chosen the following way to convert Eigen matrix to unsigned char *

	//Parse arguments
	cxxopts::Options options("MyProgram", "One line description of MyProgram");
	options.add_options()
		("i,image_path", "Path to binary input image", cxxopts::value<std::string>()) 
		("o,output_path", "Path to save result", cxxopts::value<std::string>()) ;
	auto result = options.parse(argc, argv);

	std::string filename = result["image_path"].as<std::string>(); 
	std::string output_filename  = result["output_path"].as<std::string>(); 
	std::cout<<filename<<std::endl;

	//Parse config file
	Config config;
	std::ifstream is("../src/configs.json");
	cereal::JSONInputArchive archive(is);
	archive(config);
	
	//Load image
	int x,y,n;
 	unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 0);
	if (data==NULL){
		std::cout<< "Cant load image"<<std::endl;
	}

	//Port array to Eigen matrix
	MatrixXf gray;
	convertRaw2Eigen(data,gray,x,y);
	normalise_img(gray);

	//Convert back to raw buffer 
	unsigned char * gray_UC; 
	convertMat2UC(gray,gray_UC,x*y); 
	stbi_write_jpg(output_filename.c_str(),x,y,n,gray_UC,100); 

	return 0;
}
