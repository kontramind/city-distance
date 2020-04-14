add_library(
    citylib
    city.h
    city.cpp
    )

target_include_directories(citylib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
