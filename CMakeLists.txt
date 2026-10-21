cmake_minimum_required(VERSION 3.16)
project(LizardMustDiePlatformer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(game
    src/main.cpp
    src/Infrastructure/LevelRepository.cpp
    src/App/CameraController.cpp
    src/App/Game.cpp
    src/Input/KeyboardHandler.cpp
    src/Player/Player.cpp
    src/Player/PlayerStateManager.cpp
    src/World/CollisionSystem/CollisionSystem.cpp
    src/World/LiquidSystem/LiquidSystem.cpp
    src/World/Map/Map.cpp
    src/World/World.cpp
    src/View/PlayerRenderer/PlayerRenderer.cpp
    src/View/TileRenderer/TileRenderer.cpp
    src/View/WorldRenderer/WorldRenderer.cpp
)

target_include_directories(game PRIVATE src)

target_link_libraries(game PRIVATE SFML::Graphics)

target_compile_definitions(game PRIVATE
    PLATFORMER_LEVELS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/levels"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:game>/assets"
    )
endif()