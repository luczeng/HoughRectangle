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
        Eigen::MatrixXf hough_transform(Eigen::MatrixXf & img, int thetaBins, int rhoBins,float  thetaMin,float thetaMax);

        Eigen::MatrixXf windowed_hough(Eigen::MatrixXf & img,int r_min,int r_max, int thetaBins, int rhoBins,float  thetaMin,float thetaMax);

        Eigen::MatrixXf apply_windowed_hough(Eigen::MatrixXf & img,int L_window,int r_min,int r_max, int thetaBins, int rhoBins,float  thetaMin,float thetaMax);

        void enhance_hough(Eigen::MatrixXf & hough,Eigen::MatrixXf & houghpp,Config & config);
        Eigen::MatrixXf ring(Eigen::MatrixXf & img,int r_min,int r_max);

};

#endif
