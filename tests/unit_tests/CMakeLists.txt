#------------------------------------------------------------------#
add_executable(test_io test_io.cpp)
target_link_libraries(test_io PUBLIC Catch2::Catch2 PRIVATE stb PUBLIC hough_lib)
add_test(NAME io_lib_test COMMAND test_io)

#------------------------------------------------------------------#
set(UNIT_TEST_FOLDER_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DUNIT_TEST_FOLDER_PATH="${UNIT_TEST_FOLDER_PATH}")

add_executable(test_process_img test_process_img.cpp)
target_include_directories(test_process_img PUBLIC include)
target_link_libraries(test_process_img PUBLIC Catch2::Catch2 PRIVATE stb PUBLIC hough_lib)
add_test(NAME img_process_lib_test COMMAND test_process_img)
#------------------------------------------------------------------#
add_executable(test_rectangle_utils test_rectangle_utils.cpp)
target_link_libraries(test_rectangle_utils PUBLIC Catch2::Catch2 PRIVATE stb PUBLIC hough_lib)
add_test(NAME rectangle_utils_test COMMAND test_rectangle_utils)




