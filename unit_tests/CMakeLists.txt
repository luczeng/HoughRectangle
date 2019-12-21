add_executable(test_io test_io.cpp)
target_link_libraries(test_io PUBLIC Catch2::Catch2 PRIVATE stb PUBLIC main_hough)

add_executable(test_process_img test_process_img.cpp)
target_include_directories(test_process_img PUBLIC include)
target_link_libraries(test_process_img PUBLIC Catch2::Catch2 PRIVATE stb PUBLIC main_hough)


