#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION #include <Eigen/Dense>
#include "process_image.hpp"
#include "config.hpp"
#include "io.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"

TEST_CASE("Test functions to compute the Hough Rectangle function"){

    SECTION("Tests the ring mask"){

        //Ground truth
        Eigen::MatrixXf img = ((1+Eigen::MatrixXf::Random(7,7).array()/2)*10).round();
        Eigen::MatrixXf img_gt = img.replicate<1,1>();
        img_gt(0,0) = 0, img_gt(0,1) = 0, img_gt(0,2) = 0, img_gt(0,4) = 0, img_gt(0,5) = 0, img_gt(0,6) = 0;
        img_gt(1,6) = 0, img_gt(2,6) = 0, img_gt(4,6) = 0, img_gt(5,6) = 0, img_gt(6,6) = 0;
        img_gt(6,0) = 0, img_gt(6,1) = 0,img_gt(6,2) = 0,img_gt(6,4) = 0,img_gt(6,5) = 0,img_gt(6,6) = 0;
        img_gt(1,0) = 0, img_gt(2,0) = 0,img_gt(4,0) = 0,img_gt(5,0) = 0;
        img_gt.block<3,3>(2,2) = Eigen::MatrixXf::Zero(3,3);
        

        //Apply ring
        HoughRectangle ht(img);
        Eigen::MatrixXf img_ring = ht.ring(img,2,3);
        std::cout << img << std::endl<<std::endl;
        std::cout << img_gt << std::endl<<std::endl;
        std::cout << img_ring<< std::endl<<std::endl;

        //Matrixes equality
        REQUIRE(img_gt == img_ring);

     }

}
