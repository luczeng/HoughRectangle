add_executable(test_apps test_apps.cpp)
target_link_libraries(test_apps PUBLIC Catch2::Catch2 PUBLIC stb PUBLIC hough_lib)
add_test(NAME io_lib_test COMMAND test_apps)
