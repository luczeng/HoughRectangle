#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;

void normalise_img(MatrixXf & img){
	/*
	 * Function to make sure binary is 0 and 255
	 */

	MatrixXf high;
	high.setOnes(img.rows(),img.cols());
	high*=255;

	MatrixXf low;
	low.setZero(img.rows(),img.cols());

	MatrixXf tmp = (img.array() > 128.0).select(high,low);
	std::cout<<tmp<<std::endl;

	img = tmp;
}

