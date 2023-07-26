cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)

project(milk_chug_game)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB_RECURSE SRC_FILES "src/*.c")
add_executable(milk_chug_game ${SRC_FILES})

target_include_directories(
    milk_chug_game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(
    milk_chug_game
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
)

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})