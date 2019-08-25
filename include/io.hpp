#ifndef IO_HPP
#define IO_HPP
#include <Eigen/Dense>
#include <string>

void convertRaw2Eigen(unsigned char * &data, Eigen::MatrixXf &gray,int x,int y);
void convertMat2UC(Eigen::MatrixXf gray,unsigned char *& gray_UC,int size);
void save_image(Eigen::MatrixXf img,std::string filename,int size, int x, int y);

#endif
