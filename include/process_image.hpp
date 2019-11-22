#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H
#include <Eigen/Dense>
#include "config.hpp"

//void normalise_img(Eigen::MatrixXf & img);
//void hough_transform(Eigen::MatrixXf & img,Config & config);
//void windowed_hough(Eigen::MatrixXf & img, Config & Config);

class HoughRectangle{
    public:
        Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> m_img;
        HoughRectangle(Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> & img); //declaration

    public:
        Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> hough_transform(Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> & img, int thetaBins, int rhoBins,float  thetaMin,float thetaMax);

        Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> windowed_hough(Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> & img,int r_min,int r_max, int thetaBins, int rhoBins,float  thetaMin,float thetaMax);

        Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> apply_windowed_hough(Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> & img,int L_window,int r_min,int r_max, int thetaBins, int rhoBins,float  thetaMin,float thetaMax);

        Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> enhance_hough(Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> & hough,int h,int w);
        
        Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> ring(Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> & img,int r_min,int r_max);

        std::vector<Eigen::Index> find_local_maximum(Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> & img,float threshold);

};

#endif
