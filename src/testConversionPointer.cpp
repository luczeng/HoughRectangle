#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Eigen/Dense>
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"

using namespace Eigen;
int main(){
	//Prepare eigen matrix
	MatrixXd m(2,5);	
	for (int i=0; i<2;i++)
	    for (int j=0; j<5;j++)
		    m(i,j) = i+j;
	std::cout<<m<<std::endl;
	Eigen::Map<MatrixXd>(p,2,5) = m;

	//Port Eigen to data
	ouble a[10];
	double *p = &a[0];
	p = m.data();
	
	//Convert to unsigned char and port into c array
	unsigned char* pp;
	unsigned char ppp[10];
	for (int i=0;i<10;++i){
//		printf("%u",static_cast<unsigned char>(p[i]));
		ppp[i]=static_cast<unsigned char>(p[i]);
		printf("%u",static_cast<unsigned char>(ppp[i]));
	}

	std::cout<< "\n";
	pp= ppp;
	for (int i=0;i<10;i++){
		printf("%u",*(pp+i));
	}

}
