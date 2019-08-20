#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H
#include <Eigen/Dense>
#include "config.hpp"

void normalise_img(Eigen::MatrixXf & img);
void hough_transform(Eigen::MatrixXf & img,Config & config);

#endif
