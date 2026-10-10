cmake_minimum_required(VERSION 3.8)
project(sfml-platformer-iter1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System CONFIG REQUIRED)

add_executable(game
    src/main.cpp
    src/Domain/Player/Player.cpp
    src/Infrastructure/Game/Game.cpp
    src/Application/World/World.cpp
    src/Domain/Obstacle/Obstacle.cpp
    src/Domain/Liquid/Liquid.cpp
    src/Domain/Map/Map.cpp
    src/Application/LiquidSystem/LiquidSystem.cpp
    src/Application/CollisionSystem/CollisionSystem.cpp
    src/Infrastructure/KeyboardHandler/KeyboardHandler.cpp
)

target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(game PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)