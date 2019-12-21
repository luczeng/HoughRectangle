cmake_minimum_required(VERSION 3.5)
project(HoughTransform)

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED)

#----------------------------------------------------------------------------------------------------#
#External libraries
#----------------------------------------------------------------------------------------------------#
set(STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
set(CXXOPTS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cxxopts/include)
set(CEREAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cereal-master/include)
set(CATCH2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/catch2)

#Eigen
find_package (Eigen3 REQUIRED NO_MODULE)
add_subdirectory(third_party/eigen)

#Stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${STB_INCLUDE_DIR})

#Cxxopts
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE ${CXXOPTS_INCLUDE_DIR})

#Config4cpp
add_library(config4cpp INTERFACE)
target_include_directories(config4cpp INTERFACE ${CONFIG4CPP_INCLUDE_DIR})

#Cereal
add_library(cereal INTERFACE)
target_include_directories(cereal INTERFACE ${CEREAL_INCLUDE_DIR})

#Catch2
add_subdirectory(third_party/Catch2)


#----------------------------------------------------------------------------------------------------#
# Main code
#----------------------------------------------------------------------------------------------------#

add_library(main_hough STATIC src/io.cpp src/process_image.cpp include/io.hpp include/config.hpp include/process_image.hpp)
target_include_directories(main_hough PUBLIC include)
target_link_libraries(main_hough PUBLIC Eigen3::Eigen PUBLIC cereal PRIVATE stb)

#----------------------------------------------------------------------------------------------------#
# Source code
#----------------------------------------------------------------------------------------------------#
add_subdirectory(apps)

#----------------------------------------------------------------------------------------------------#
# Tests
#----------------------------------------------------------------------------------------------------#
add_executable(test_io unit_test/test_io.cpp)
target_link_libraries(test_io PUBLIC Catch2::Catch2 PRIVATE stb PUBLIC main_hough)

add_executable(test_process_img unit_test/test_process_img.cpp)
target_include_directories(test_process_img PUBLIC include)
target_link_libraries(test_process_img PUBLIC Catch2::Catch2 PRIVATE stb PUBLIC main_hough)

