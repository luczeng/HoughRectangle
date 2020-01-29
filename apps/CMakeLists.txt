add_executable(find_pairs find_pairs.cpp)
target_link_libraries(find_pairs PUBLIC Eigen3::Eigen PUBLIC hough_lib PRIVATE  cxxopts PRIVATE stb PUBLIC cereal)

add_executable(main_hough_rectangle main_hough_rectangle.cpp )
target_link_libraries(main_hough_rectangle PUBLIC Eigen3::Eigen PUBLIC hough_lib PUBLIC cxxopts PRIVATE stb PUBLIC cereal)

add_executable(main_fast_rectangle_detect main_fast_rectangle_detect.cpp )
target_link_libraries(main_fast_rectangle_detect PUBLIC Eigen3::Eigen PUBLIC hough_lib PUBLIC cxxopts PRIVATE stb PUBLIC cereal)

add_executable(main_hough_rectangle_centered main_hough_rectangle_centered.cpp )
target_link_libraries(main_hough_rectangle_centered PUBLIC Eigen3::Eigen PUBLIC hough_lib PUBLIC cxxopts PRIVATE stb PUBLIC cereal)

#add_executable(windowed_hough windowed_hough.cpp)
#target_link_libraries(windowed_hough PUBLIC Eigen3::Eigen PUBLIC hough_lib  PUBLIC cxxopts PRIVATE stb PRIVATE cereal)

#add_executable(hough_transform hough_transform.cpp)
#target_link_libraries(hough_transform PUBLIC Eigen3::Eigen PUBLIC hough_lib PUBLIC cxxopts PRIVATE stb PRIVATE cereal)

#add_executable(find_maximums find_maximums.cpp)
#target_link_libraries(find_maximums PUBLIC Eigen3::Eigen PUBLIC hough_lib PUBLIC cxxopts PRIVATE stb PRIVATE cereal)
