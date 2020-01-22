#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Test that the apps can execute") {

    SECTION("Test that the main script runs") {
        system("apps/main_hough_rectangle -i ../img/img/img/rectangle2jhg.png -o rectangles.txt");
    }

}

