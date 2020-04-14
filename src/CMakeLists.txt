add_executable(distances main.cpp)

target_link_libraries(
                        distances
                        PRIVATE
                        project_options
                        project_warnings
                        citylib
                        yaml-cpp
)
