cmake_minimum_required(VERSION 3.5)
project(HoughTransform)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED)

#----------------------------------------------------------------------------------------------------#
#External libraries
#----------------------------------------------------------------------------------------------------#
set(STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
set(CXXOPTS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cxxopts/include)
set(CEREAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cereal-master/include)
#message(${CMAKE_CURRENT_SOURCE_DIR})
#message(${STB_INCLUDE_DIR})

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

#----------------------------------------------------------------------------------------------------#
# Main code
#----------------------------------------------------------------------------------------------------#

add_library(main_hough STATIC src/io.cpp src/process_image.cpp include/io.hpp include/process_image.hpp)
target_include_directories(main_hough PUBLIC include)
target_link_libraries(main_hough PUBLIC Eigen3::Eigen)
#target_include_directories(io PUBLIC ${STB_INCLUDE_DIR})


#----------------------------------------------------------------------------------------------------#
#Executables
#----------------------------------------------------------------------------------------------------#
add_executable(HoughRectangle src/HoughRectangle.cpp )
target_link_libraries(HoughRectangle PUBLIC Eigen3::Eigen PUBLIC main_hough PUBLIC cxxopts PRIVATE stb PUBLIC cereal)
#target_include_directories(HoughRectangle PRIVATE ${STB_INCLUDE_DIR} PUBLIC ${CXXOPTS_INCLUDE_DIR})

