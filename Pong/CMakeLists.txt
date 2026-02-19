cmake_minimum_required(VERSION 3.28)
project(Pong LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(aurora-engine CONFIG REQUIRED)

add_executable(Pong main.cpp)

target_link_libraries(Pong
    PRIVATE
        aurora-engine::aurora-engine
)
