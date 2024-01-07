cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 23)

project(milk_chug_game)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB_RECURSE SRC_FILES "src/*.c")
add_executable(milk_chug_game ${SRC_FILES})

target_include_directories(
    milk_chug_game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(
    milk_chug_game
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)

install(TARGETS milk_chug_game RUNTIME DESTINATION bin)
install(DIRECTORY assets/ DESTINATION share/assets)
install(IMPORTED_RUNTIME_ARTIFACTS
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    DESTINATION lib
)
install(FILES resources/milk_chug.desktop DESTINATION share/applications)
install(FILES resources/milk_chug.png DESTINATION share/icons/hicolor/32x32/apps)
