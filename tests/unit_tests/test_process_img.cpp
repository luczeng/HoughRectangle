#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION #include <Eigen/Dense>
#include <array>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include "io.hpp"
#include "process_image.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"
#include "rectangle_detection.hpp"
#include "eigen_utils.hpp"

TEST_CASE("Test functions to compute the Hough Rectangle function") {
    SECTION("Tests the ring mask") {
        // Input
        Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> img =
            ((1 + Eigen::MatrixXf::Random(7, 7).array() / 2) * 10).round();

        // Ground truth
        Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> img_gt = img.replicate<1, 1>();

        img_gt(0, 0) = 0, img_gt(0, 1) = 0, img_gt(0, 2) = 0, img_gt(0, 4) = 0, img_gt(0, 5) = 0, img_gt(0, 6) = 0;
        img_gt(1, 6) = 0, img_gt(2, 6) = 0, img_gt(4, 6) = 0, img_gt(5, 6) = 0, img_gt(6, 6) = 0;
        img_gt(6, 0) = 0, img_gt(6, 1) = 0, img_gt(6, 2) = 0, img_gt(6, 4) = 0, img_gt(6, 5) = 0, img_gt(6, 6) = 0;
        img_gt(1, 0) = 0, img_gt(2, 0) = 0, img_gt(4, 0) = 0, img_gt(5, 0) = 0;
        img_gt.block<3, 3>(2, 2) = Eigen::MatrixXf::Zero(3, 3);

        // Apply ring
        HoughRectangle ht(img);
        Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> img_ring = ht.ring(img, 2, 3);
        std::cout << img << std::endl << std::endl;
        std::cout << img_gt << std::endl << std::endl;
        std::cout << img_ring << std::endl << std::endl;

        // Matrixes equality
        REQUIRE(img_gt == img_ring);
    }

    SECTION("Test the local maximum detection") {
        // Input
        Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> img(4, 5);
        img << 1, 2, 4, 5, 5, 5, 3, 9, 2, 5, 4, 2, 6, 5, 9, 3, 9, 1, 8, 2;

        // Ground truth
        std::vector<std::array<int, 2>> idxs;
        idxs.push_back({1, 2});
        idxs.push_back({2, 4});
        idxs.push_back({3, 1});
        idxs.push_back({3, 3});

        // HoughRectangle ht(img);
        std::vector<std::array<int, 2>> max_pos = find_local_maximum(img, 8);

        REQUIRE(idxs == max_pos);
    }

    SECTION("Test pair matching") {
        // Input data
        std::vector<float> rho_vec = {50, -49, 10, 20, -48};
        std::vector<float> theta_vec = {45, 10, 15, 17, 44};

        // Pair matcher
        auto pairs = rectangle_detect::find_pairs(rho_vec, theta_vec, 2.1, 1.1, 2);

        // GT
        std::vector<std::array<float, 4>> pairs_gt;
        std::array<float, 4> pair_gt = {49, 44.5, 2, 1};
        pairs_gt.push_back(pair_gt);

        REQUIRE(pairs_gt == pairs);
    }

    SECTION("Test duplicate removal") {
        // Input data
        std::vector<std::array<float, 8>> rectangles;
        std::array<float, 8> rect1 = {40, 20, 29, 1, 1, 2, 2, 1};
        std::array<float, 8> rect2 = {28, 19, 19, 2, 3, 1, 2, 9};

        rectangles.push_back(rect1);
        rectangles.push_back(rect2);

        // Remove duplicates
        auto rect = rectangle_detect::remove_duplicates(rectangles, 1, 1);

        REQUIRE(rect == rect1);
    }

    SECTION("Test the peak detection") {
        std::string test_img_file_path = UNIT_TEST_FOLDER_PATH;
        HoughRectangle::fMat gray = eigen_io::read_image(test_img_file_path + "/../../img/rectangle1.png");

        // Parameters
        int thetaBins = 256;
        int thetaMin = -90;
        int thetaMax = 90;
        int rhoBins = 256;

        // Process image
        HoughRectangle ht(gray, thetaBins, rhoBins, thetaMin, thetaMax);
        HoughRectangle::fMat wht = ht.hough_transform(gray);

        // Detect peaks
        std::vector<std::array<int, 2>> indexes = find_local_maximum(wht, 50);
        std::vector<float> rho_maxs, theta_maxs;
        std::tie(rho_maxs, theta_maxs) = ht.index_rho_theta(indexes);
        std::cout << "Detected " << indexes.size() << " points" << std::endl;

        // for (int i = 0; i < rho_maxs.size(); ++i) {
        // std::cout << i << " " << rho_maxs[i] << " " << theta_maxs[i]
        //<< std::endl;
        //}

        // Match peaks into rectangles
        // std::vector<std::array<float, 3>> rectangles = ht.match_maximums(rho_maxs, theta_maxs, 1, 1, 30, 3);

        // std::cout << "Found " << rectangles.size() << " rectangles" << std::endl;

        // for (auto rect : rectangles) {
        // std::cout << rect[0] << " " << rect[1] << " " << rect[2]
        //<< std::endl;
        //}

        // std::vector<std::array<int, 8>> rectangles_cart =
        // convert_all_rects_2_cartesian(rectangles,
        // 128,128);

        // save_rectangle("rectangles.txt", rectangles_cart);
    }
}
