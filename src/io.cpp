#include "io.hpp"
#include <iostream>
#include "string"
#include <Eigen/Dense>
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image.h"
//#include "stb_image_write.h"

using namespace Eigen;

void convertRaw2Eigen(unsigned char * &data, MatrixXf &gray,int x,int y){
	const int nx = x;
	const int ny = y;
	typedef Matrix<unsigned char,Dynamic, Dynamic> MatrixXUC;
	typedef Map<MatrixXUC,RowMajor> MapType;
	MapType img(data,y,x);
	gray = img.cast<float>();
}

void convertMat2UC(MatrixXf gray,unsigned char * &gray_UC,int size)
{
	unsigned char gray_UC_tmp[size];
	float *gray_array = gray.data();
	std::cout<<size<<std::endl;
	for (int i=0; i<size;++i){
		gray_UC_tmp[i] = static_cast<unsigned char> (gray_array[i]);	
	}
	
	gray_UC = gray_UC_tmp;
}

//#endif
//#endif
