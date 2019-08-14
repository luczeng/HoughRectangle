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

	img = tmp;
}

void hough_transform(MatrixXi & img,VectorXf theta,VectorXf rho){
	MatrixXi acc; //accumulator

	//Compute Hough transform
	for (int i=0;i<img.rows();++i){
		for (int j=0;j<img.cols();++j){
			if (img(i,j) !=0){
			//generate sinusoidal curve	
			}
		}
	}
}

