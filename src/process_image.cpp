#include <Eigen/Dense>
#include <iostream>
#include "config.hpp"
#include <math.h>
#include <stdlib.h>
#include "io.hpp"
#include "stb_image.h"
#include "stb_image_write.h"

#define PI 3.14159265

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

std::vector<float> LinearSpacedArray(float a, float b, std::size_t N)
	/*
	 * Returns a linearly spaced array
	 */
	{
	double h = (b - a) / static_cast<float>(N-1);
	std::vector<float> xs(N);
	std::vector<float>::iterator x;
	float val;
	for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h) {
	*x = val;
	}
	return xs;
	}

void hough_transform(MatrixXf & img,Config & config){
	/*
	 * Performs the Hough trasnform on the input edge detected matrix
	 */

	//Define accumulator matrix, theta and rho vectors
	MatrixXf acc = MatrixXf::Zero(config.thetaBins,config.rhoBins); //accumulator
	VectorXf theta = VectorXf::LinSpaced(Sequential,config.thetaBins,config.thetaMin,config.thetaMax);
	std::vector<float> rho = LinearSpacedArray(-360,sqrt(pow(img.rows()/2.0,2) + pow(img.rows()/2.0,2)),config.rhoBins);
	std::cout<< rho[config.rhoBins-1]<<std::endl;

	//Cartesian coordinate vectors
	VectorXi vecX = VectorXi::LinSpaced(Sequential,img.cols(),0,img.cols()-1);
	VectorXi vecY = VectorXi::LinSpaced(Sequential,img.rows(),0,img.rows()-1);
	int mid_X =round(img.cols()/2);; 
	int mid_Y =round(img.rows()/2);; 
	vecX = vecX.array() - mid_X; 
	vecY = vecY.array() - mid_Y; 

	//Pre-compute cosines and sinuses:
	VectorXf cosT = cos(theta.array()*PI/180.0);
	VectorXf sinT = sin(theta.array()*PI/180.0);

	//Compute Hough transform
	for (int i=0;i<img.rows();++i){
		for (int j=0;j<img.cols();++j){ 
			if (img(i,j) !=0){
			//generate sinusoidal curve	
				for (int k=0;k<theta.size();++k){
					//Calculate rho value
					float rho_tmp = (vecX[j]*cosT[k] + vecY[i]*sinT[k]);

					std::vector<float>::iterator idx;
					idx = std::lower_bound(rho.begin(),rho.end(),rho_tmp);
					int idx_rho = idx - rho.begin()-1;
					//std::cout <<rho_tmp<<std::endl;
					if (idx_rho<0)
					{
						idx_rho = 0;
					}

					//Fill accumulator
					acc(k,idx_rho) = acc(k,idx_rho) + 1;
					if (acc(k,idx_rho) > pow(2,32)){std::cout<< "Max value overpassed";}
				}
			}
		}
	}

	//The following is to save the sinuoigram. Shall be moved later on
	//Normalise to 0-255
	acc = acc / acc.maxCoeff() *255;
	acc = acc.array().ceil();

	//Convert to unsigned char and save
//	unsigned char * gray_UC_hough; 
//	int n = config.rhoBins*config.thetaBins;
//	convertMat2UC(acc,gray_UC_hough,n); 
//	int success = stbi_write_png("accumulator.jpg",config.thetaBins,config.rhoBins,1,gray_UC_hough,config.rhoBins); 
//	std::cout<< "Accumulator saved: "<< success << std::endl;
}	
