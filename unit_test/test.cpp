#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "io.hpp"
#include "string"

unsigned char uc_conv(int x){return static_cast<unsigned char>(x);};

//Raw buffer matcher
class RawBufEq : public Catch::MatcherBase<unsigned char *> {
	unsigned char * m_arr1;
	int m_L;

	public:
		RawBufEq(unsigned char * arr1,int L) : m_arr1( arr1), m_L(L) {}

		//Matcher. If elements don't match, prints them
		bool match(unsigned char * const& arr2) const override {
			bool out = true;
			for (int i=0;i<m_L;i++){
				if (m_arr1[i] != arr2[i])
				{
					std::cout<<"elements "<<i<<": arr1="<<(int)m_arr1[i] <<", arr2="<<(int)arr2[i]<<std::endl;
					out = false;
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

inline RawBufEq IsEqual(unsigned char * arr1,int L){
	return RawBufEq(arr1,L);
}

//Tests
TEST_CASE("Test Input-output functions for images"){

	std::string filename = "../unit_test/test_img.png";

	SECTION("Image reader into Eigen matrix"){
		int x, y, n;

		unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 0);
		Eigen::MatrixXf gray;
		convertRaw2Eigen(data, gray, x, y);

		Eigen::MatrixXf ground_truth(3,4);
		ground_truth << 1, 2, 3,155,
		     255,3, 4, 5,
		     2, 5,78, 1;

		REQUIRE( data != NULL); //sanity check
		REQUIRE( ground_truth == gray);

		delete data;
	}

	SECTION("Converter from Eigen matrix to unsigned char raw buffer"){

		//Build input
		Eigen::Matrix<float,3,4,Eigen::RowMajor> gray;;
		gray << 1.34,3.908,55.201,255.0978,
		     0.097,69.698,25.09853,94.8975,
		     30.309,50.5,38.0985,50;

		int ground_truth_int[3*4] = { 1,4,55,255,
		     0,70,25,95,
		     30,51,38,50};

		//Populate GT. Replace this loop later
		unsigned char tmp[12];
		unsigned char * ground_truth;
		for (int i=0;i<12;i++){
			tmp[i] = static_cast<unsigned char> (ground_truth_int[i]);	
		}
		ground_truth = tmp;


		//Convert matrix to raw buffer
		unsigned char * gray_UC;
		convertMat2UC(gray,gray_UC,3*4);

		//Test Raw buffers equality
		REQUIRE_THAT( gray_UC, IsEqual(ground_truth,12));

		delete gray_UC;

	}
}

