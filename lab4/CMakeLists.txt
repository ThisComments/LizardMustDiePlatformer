cmake_minimum_required(VERSION 3.8)
project(sfml-test)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 COMPONENTS Graphics Window System CONFIG REQUIRED)

add_executable(app src/main.cpp)

target_link_libraries(app PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)