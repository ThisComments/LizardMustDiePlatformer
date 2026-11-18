cmake_minimum_required(VERSION 3.16)
project(LizardMustDiePlatformer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(TagLib CONFIG REQUIRED)

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(game ${GAME_SOURCES})

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