#include <Eigen/Dense>
void convertRaw2Eigen(unsigned char * &data, Eigen::MatrixXf &gray,int x,int y);
void convertMat2UC(Eigen::MatrixXf gray,unsigned char *& gray_UC,int size);
