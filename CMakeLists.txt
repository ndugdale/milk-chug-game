cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 17)

project(MilkChug VERSION 1.0.0)

if(WIN32)
    # SDL Libraries must be installed in this directory or under the existing CMake search path
    set(CMAKE_PREFIX_PATH "C:")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB_RECURSE SRC_FILES "src/*.c")

if(DEFINED ENV{PACKAGE_APP})
    add_compile_definitions(PACKAGE_APP)
endif()

# Create target
if(APPLE AND DEFINED ENV{PACKAGE_APP})
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
    set(CMAKE_BUILD_RPATH "@executable_path/../Frameworks")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/install)
    add_executable(MilkChug
        MACOSX_BUNDLE
        ${SRC_FILES}
    )
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    add_executable(MilkChug
        ${SRC_FILES}
    )
endif()

# Include custom headers
target_include_directories(MilkChug PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link to libraries
if(WIN32)
    target_link_libraries(MilkChug
        SDL2::SDL2main
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
    )
    # On Windows install dll files to bin directory
    install(IMPORTED_RUNTIME_ARTIFACTS
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
        DESTINATION $<TARGET_FILE_DIR:MilkChug>
    )
else()
    target_link_libraries(MilkChug
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
    )
endif()

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

    install(DIRECTORY assets/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/Resources/assets)
    install(FILES resources/macos/milk_chug.icns DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/Resources)
    install(DIRECTORY
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/Frameworks
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
    install(FILES resources/linux/milk_chug.desktop DESTINATION share/applications)
    install(FILES resources/linux/milk_chug.png DESTINATION share/icons/hicolor/32x32/apps)
endif()
