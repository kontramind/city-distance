add_library(
    citylib
    city.h
    city.cpp
    )

target_include_directories(citylib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(cityTest
               test/test-main.cpp
               test/cityTest.cpp)
target_link_libraries(cityTest Catch2::Catch2 citylib yaml-cpp)
enable_testing()
add_test(NAME cityTest COMMAND cityTest)
