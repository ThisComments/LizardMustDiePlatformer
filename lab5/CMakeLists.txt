cmake_minimum_required(VERSION 3.8)
project(sfml-platformer-iter1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System CONFIG REQUIRED)

add_executable(game
    src/main.cpp
    src/Player/Player.cpp
    src/Game/Game.cpp
    src/World/World.cpp
    src/Obstacle/Obstacle.cpp
    src/CollisionSystem/CollisionSystem.cpp
    src/KeyboardHandler/KeyboardHandler.cpp
)

target_include_directories(game PRIVATE 
    src/Player
    src/Game
    src/World
    src/Obstacle
    src/CollisionSystem
    src/KeyboardHandler
)

target_link_libraries(game PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)