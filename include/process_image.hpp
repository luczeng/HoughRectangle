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
        HoughRectangle(Eigen::MatrixXf & img); //declaration

    public:
        void hough_transform(Eigen::MatrixXf & img,Config & config);
        void windowed_hough(Eigen::MatrixXf & img,int r_min,int r_max);
        void enhance_hough(Eigen::MatrixXf & hough,Eigen::MatrixXf & houghpp,Config & config);
        Eigen::MatrixXf ring(Eigen::MatrixXf & img,int r_min,int r_max);

};

#endif
