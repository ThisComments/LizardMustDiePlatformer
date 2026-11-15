cmake_minimum_required(VERSION 3.16)
project(LizardMustDiePlatformer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(TagLib CONFIG REQUIRED)

add_executable(game
    src/main.cpp
    src/Animation/AnimationComponent.cpp
    src/Audio/AudioManager.cpp
    src/Resources/ResourceManager.cpp
    src/Model/Collectible/Collectible.cpp
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
    src/Systems/TriggerSystem/TriggerSystem.cpp
    src/World/World.cpp
    src/View/MapRenderer/MapRenderer.cpp
    src/View/WorldRenderer/WorldRenderer.cpp
    src/View/ParallaxBackground/ParallaxBackground.cpp
)

target_include_directories(game PRIVATE src)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(game PRIVATE -Werror=switch)
endif()

target_link_libraries(game PRIVATE
    SFML::Graphics
    SFML::Audio
    tomlplusplus::tomlplusplus
    TagLib::tag
)

set(GAME_DATA_DIRS assets settings levels)
set(GAME_COPY_COMMANDS "")
foreach(dir IN LISTS GAME_DATA_DIRS)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
        set(copy_mode copy_directory_if_different)
        list(APPEND GAME_COPY_COMMANDS
            COMMAND ${CMAKE_COMMAND} -E ${copy_mode}
                "${CMAKE_CURRENT_SOURCE_DIR}/${dir}" "$<TARGET_FILE_DIR:game>/${dir}")
    endif()
endforeach()

if(GAME_COPY_COMMANDS)
    add_custom_target(copy_game_data ALL ${GAME_COPY_COMMANDS} VERBATIM)
    add_dependencies(copy_game_data game)
endif()