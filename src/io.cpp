#include "io.hpp"
#include <iostream>
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Eigen/Dense>
//#include "stb_image.h"
//#include "stb_image_write.h"
#include "string"

using namespace Eigen;
void convertMat2UC(MatrixXf gray,unsigned char * &gray_UC,int size)
{
	unsigned char gray_UC_tmp[size];
	float *gray_array = gray.data();
	for (int i=0; i<size;++i){
		gray_UC_tmp[i] = static_cast<unsigned char> (gray_array[i]);	
	}
	gray_UC = gray_UC_tmp;
}

//#endif
//#endif
