#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "io.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "string"

unsigned char uc_conv(int x) { return static_cast<unsigned char>(x); };

/////////////////////////////////////////////////////////////////////////////
// Raw buffer matcher
/////////////////////////////////////////////////////////////////////////////
class RawBufEq : public Catch::MatcherBase<unsigned char*> {
    unsigned char* m_arr1;
    int m_L;

   public:
    RawBufEq(unsigned char* arr1, int L) : m_arr1(arr1), m_L(L) {}

    // Matcher. If elements don't match, prints them
    bool match(unsigned char* const& arr2) const override {
        bool out = true;
        for (int i = 0; i < m_L; i++) {
            if (m_arr1[i] != arr2[i]) {
                std::cout << "elements " << i << ": arr1=" << (int)m_arr1[i]
                          << ", arr2=" << (int)arr2[i] << std::endl;
                out = false;
            } else {
                std::cout << "elements " << i << ": arr1=" << (int)m_arr1[i]
                          << ", arr2=" << (int)arr2[i] << std::endl;
            }
        }
        return out;
    }

    virtual std::string describe() const override {
        std::ostringstream ss;
        ss << "Arrays do not match";

        return ss.str();
    }
};

inline RawBufEq IsEqual(unsigned char* arr1, int L) {
    return RawBufEq(arr1, L);
}

/////////////////////////////////////////////////////////////////////////////
// Tests
/////////////////////////////////////////////////////////////////////////////
TEST_CASE("Test Input-output functions for images") {
    std::string filename = "../unit_test/test_img.png";

    SECTION("Image reader into Eigen matrix") {
        int x, y, n;

        // unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n,
        // 0);
        std::unique_ptr<unsigned char[]> data_tmp(
            stbi_load(filename.c_str(), &x, &y, &n, 0));

        Eigen::MatrixXf gray;

        // Function to be tested
        convert_RawBuff2Mat(data_tmp, gray, x, y);

        // Ground truth
        Eigen::MatrixXf ground_truth(3, 4);
        ground_truth << 1, 2, 3, 155, 255, 3, 4, 5, 2, 5, 78, 1;

        // REQUIRE( data_tmp != NULL); //sanity check
        REQUIRE(ground_truth == gray);
    }

    SECTION("Converter from Eigen matrix to unsigned char raw buffer") {
        // Build input
        const int x = 4;
        const int y = 3;
        const int L = x * y;
        Eigen::Matrix<float, y, x, Eigen::RowMajor> gray;
        ;
        gray << 1.34, 3.908, 55.201, 255.0978, 0.097, 69.698, 25.09853, 94.8975,
            30.309, 50.5, 38.0985, 50;

        int ground_truth_int[L] = {1,  4,  55, 255, 0,  70,
                                   25, 95, 30, 51,  38, 50};

        // Populate GT. Replace this loop later
        std::unique_ptr<unsigned char[]> ground_truth(new unsigned char[L]);
        for (int i = 0; i < L; i++) {
            ground_truth[i] = static_cast<unsigned char>(ground_truth_int[i]);
        }

        // Function to test: converting matrix to raw buffer
        std::unique_ptr<unsigned char[]> gray_UC(new unsigned char[L]);
        convert_Mat2RawBuff(gray, gray_UC, 3 * 4);

        // Test Raw buffers equality
        REQUIRE_THAT(gray_UC.get(), IsEqual(ground_truth.get(), 12));
    }

    SECTION("Eigen matrix saver") {
        // Dummy matrix
        Eigen::Matrix<float, 3, 6, Eigen::RowMajor> gray;
        gray << 2, 3, 1, 5, 6, 5, 5, 2, 6, 1, 9, 3, 3, 1, 5, 3, 2, 1;

        auto out = save_image(gray, "../unit_test/unit_test.png", 30, 6, 3);

        REQUIRE(out == 1);
    }
}

