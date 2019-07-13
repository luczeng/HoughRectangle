cmake_minimum_required(VERSION 3.5)
project(HoughTransform)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED)

set(STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb)
set(CXXOPTS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cxxopts/include)
#message(${CMAKE_CURRENT_SOURCE_DIR})
#message(${STB_INCLUDE_DIR})

#Eigen
find_package (Eigen3 REQUIRED NO_MODULE)
add_subdirectory(eigen)

#Stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${STB_INCLUDE_DIR})

#Cxxopts
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE ${CXXOPTS_INCLUDE_DIR})

add_library(io STATIC src/io.cpp include/io.hpp)
target_include_directories(io PUBLIC include)
target_link_libraries(io PUBLIC Eigen3::Eigen)
#target_include_directories(io PUBLIC ${STB_INCLUDE_DIR})

add_executable(HoughRectangle src/HoughRectangle.cpp )
target_link_libraries(HoughRectangle PUBLIC Eigen3::Eigen PUBLIC io PUBLIC cxxopts PRIVATE stb)
#target_include_directories(HoughRectangle PRIVATE ${STB_INCLUDE_DIR} PUBLIC ${CXXOPTS_INCLUDE_DIR})

