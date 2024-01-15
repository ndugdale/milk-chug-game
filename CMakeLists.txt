cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 23)

project(MilkChug VERSION 1.0.0)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB_RECURSE SRC_FILES "src/*.c")

if(DEFINED ENV{PACKAGE_APP})
    add_compile_definitions(PACKAGE_APP)
endif()

if(APPLE AND DEFINED ENV{PACKAGE_APP})
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
    set(CMAKE_BUILD_RPATH "@executable_path/../Frameworks")
    add_executable(MilkChug
        MACOSX_BUNDLE
        ${SRC_FILES}
    )
else()
    add_executable(MilkChug
        ${SRC_FILES}
    )
endif()

target_include_directories(MilkChug PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MilkChug
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)


# Install OS X App
if(APPLE AND DEFINED ENV{PACKAGE_APP})
    find_library(SDL2_LIBRARY SDL2)
    find_library(SDL2_IMAGE_LIBRARY SDL2_IMAGE)
    find_library(SDL2_MIXER_LIBRARY SDL2_MIXER)
    find_library(SDL2_TTF_LIBRARY SDL2_TTF)

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${CMAKE_PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${CMAKE_PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER com.ndugdale.${PROJECT_NAME}
        MACOSX_BUNDLE_ICON_FILE milk_chug
    )

    install(DIRECTORY assets/ DESTINATION ${PROJECT_NAME}.app/Contents/Resources/assets)
    install(FILES resources/milk_chug.icns DESTINATION ${PROJECT_NAME}.app/Contents/Resources)
    install(DIRECTORY
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        DESTINATION ${PROJECT_NAME}.app/Contents/Frameworks
    )


# Install AppDir for Linux AppImage
elseif(UNIX AND DEFINED ENV{PACKAGE_APP})
    install(TARGETS MilkChug RUNTIME DESTINATION bin)
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
endif()
