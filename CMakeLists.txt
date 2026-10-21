cmake_minimum_required(VERSION 3.8)
project(sfml-platformer-iter1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System CONFIG REQUIRED)

add_executable(game
    src/main.cpp
    src/App/Game.cpp
    src/Input/KeyboardHandler.cpp
    src/Player/Player.cpp
    src/Player/PlayerStateManager.cpp
    src/World/CollisionSystem/CollisionSystem.cpp
    src/World/LiquidSystem/LiquidSystem.cpp
    src/World/Map/Map.cpp
    src/World/Render/Render.cpp
    src/World/World.cpp

)

target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(game PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)