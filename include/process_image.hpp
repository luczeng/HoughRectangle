#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H
#include <Eigen/Dense>
#include "config.hpp"

//void normalise_img(Eigen::MatrixXf & img);
//void hough_transform(Eigen::MatrixXf & img,Config & config);
//void windowed_hough(Eigen::MatrixXf & img, Config & Config);

class HoughRectangle{
    public:
        Eigen::MatrixXf m_img;

    public:
        void hough_transform(Eigen::MatrixXf & img,Config & config);
        void windowed_hough(Eigen::MatrixXf & img,Config & config);
        void enhance_hough(Eigen::MatrixXf & hough,Eigen::MatrixXf & houghpp,Config & config);

};

#endif
