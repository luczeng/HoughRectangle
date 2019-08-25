#include "io.hpp"
#include <iostream>
#include "string"
#include <Eigen/Dense>
#include "stb_image_write.h"
using namespace Eigen;

void convertRaw2Eigen(unsigned char * &data, MatrixXf &gray,int x,int y){
	const int nx = x;
	const int ny = y;
	typedef Matrix<unsigned char,Dynamic, Dynamic> MatrixXUC;
	//typedef Map<MatrixXUC,RowMajor> MapType;
	typedef Map<MatrixXUC> MapType;
	MapType img(data,y,x);
	gray = img.cast<float>();
}

void convertMat2UC(MatrixXf gray,unsigned char * &gray_UC,int size)
{
	unsigned char gray_UC_tmp[size];
	float *gray_array = gray.data();

	for (int i=0; i<size;++i){
		gray_UC_tmp[i] = static_cast<unsigned char> (gray_array[i]);	
	}
	
	gray_UC = gray_UC_tmp;
}

void save_image(MatrixXf img,std::string filename,int size,int x, int y)
{
    unsigned char * gray_UC_hough; 
    convertMat2UC(img,gray_UC_hough,size); 
    int success = stbi_write_png(filename.c_str(),x,y,1,gray_UC_hough,x); 
    std::cout<< "Image saved: "<< success << std::endl;

}
