#!/bin/sh

initial_directory=$(pwd)
OS=$(uname -s)

mkdir -p build
cd build

export PACKAGE_APP="1"

# Build Linux AppImage
if [ "$OS" == "Linux" ]; then
    # Install project to AppDir for AppImage packaging
    cmake ..
    cmake --install . --prefix AppDir/usr

    # Build AppImage
    linuxdeploy_filename="linuxdeploy-x86_64.AppImage"

    if [ -x "$linuxdeploy_filename" ]; then
        echo "$linuxdeploy_filename is already installed"
    else
        echo "Installing $linuxdeploy_filename"
        wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/$linuxdeploy_filename
        chmod +x $linuxdeploy_filename
    fi

    ./$linuxdeploy_filename --appdir AppDir --output appimage
fi

# Build OS X App
if [ "$OS" == "Darwin" ]; then
    cmake ..
    cmake --build .
    cmake --install . --prefix .

    # Rename package and zip
    extension=".app"
    architecture=$(uname -m)

    for app in *"$extension"; do
        [ -d "$app" ] || break
        if [[ "$app" == *"$architecture"* ]]; then
            continue
        fi

        base_name="${app%$extension}"
        new_name="${base_name}-${architecture}${extension}"
        rm -rf "$new_name"
        mv "$app" "$new_name"
        ditto -c -k --keepParent "$new_name" "${base_name}-${architecture}.zip"
    done

fi

cd "$initial_directory"
