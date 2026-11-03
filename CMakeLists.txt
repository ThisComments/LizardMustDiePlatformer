cmake_minimum_required(VERSION 3.16)
project(LizardMustDiePlatformer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(game
    src/main.cpp
    src/Infrastructure/LevelRepository/LevelRepository.cpp
    src/Infrastructure/Input/KeyboardHandler.cpp
    src/App/CameraController/CameraController.cpp
    src/App/Game/Game.cpp
    src/Screens/ScreenManager/ScreenManager.cpp
    src/Screens/MainMenuScreen/MainMenuScreen.cpp
    src/Screens/GameplayScreen/GameplayScreen.cpp
    src/Screens/PauseScreen/PauseScreen.cpp
    src/Screens/GameOverScreen/GameOverScreen.cpp
    src/Screens/HowToPlayScreen/HowToPlayScreen.cpp
    src/UI/TextUtils/TextUtils.cpp
    src/UI/Button/Button.cpp
    src/UI/Menu/Menu.cpp
    src/UI/HUD/HUD.cpp
    src/UI/Overlay/ScreenOverlay.cpp
    src/Model/Map/Map.cpp
    src/Model/Player/Player.cpp
    src/Model/Player/PlayerStateManager/PlayerStateManager.cpp
    src/Systems/CollisionSystem/CollisionSystem.cpp
    src/Systems/LiquidSystem/LiquidSystem.cpp
    src/Systems/RenderDataBuilder/RenderDataBuilder.cpp
    src/World/World.cpp
    src/View/EntityRenderer/EntityRenderer.cpp
    src/View/MapRenderer/MapRenderer.cpp
    src/View/WorldRenderer/WorldRenderer.cpp
    src/View/ParallaxBackground/ParallaxBackground.cpp
)

target_include_directories(game PRIVATE src)

target_compile_options(game PRIVATE -Werror=switch)

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