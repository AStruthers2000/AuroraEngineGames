cmake_minimum_required(VERSION 3.28)
project(Pong LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(aurora-engine CONFIG REQUIRED)

add_executable(Pong src/main.cpp
        src/wall.cpp
        src/wall_manager.cpp
        src/ball.cpp
        src/player.cpp
        src/field.cpp
        src/better_game_object.cpp
        src/collider.cpp
        src/game_mode.cpp)

# Include directories
target_include_directories(Pong
    PUBLIC
        ${PROJECT_SOURCE_DIR}/public
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)


target_link_libraries(Pong
    PRIVATE
        aurora-engine::aurora-engine
)
