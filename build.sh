#!/bin/sh

initial_directory=$(pwd)
script_directory="$(dirname "$0")"
cd "$script_directory"
OS=$(uname -s)

mkdir -p build
cd build

export PACKAGE_APP="1"

# Build Linux AppImage
if [ "$OS" == "Linux" ]; then
    # Install project to AppDir for AppImage packaging
    cmake ..
    cmake --build .
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

    # Rename
    extension=".AppImage"
    architecture=$(uname -m)

    for app_image in *"$extension"; do
        [ -f "$app_image" ] || break
        if [[ "$app_image" == "$linuxdeploy_filename" ]]; then
            continue
        fi

        base_name=$(echo "${app_image%-$architecture$extension}" | tr -d '_')
        new_name="${base_name}-${architecture}${extension}"
        mv "$app_image" "$new_name"
    done
fi

# Build OS X App
if [ "$OS" == "Darwin" ]; then
    cmake ..
    cmake --build .
    cmake --install . --prefix .

    # Rename package and zip
    extension=".app"
    architecture=$(uname -m)

    for app in ../install/*"$extension"; do
        [ -d "$app" ] || break
        if [[ "$app" == *"$architecture"* ]]; then
            continue
        fi

        base_name="${app%$extension}"
        new_name="${base_name}-MacOS-${architecture}${extension}"
        rm -rf "$new_name"
        mv "$app" "$new_name"
        ditto -c -k --keepParent "$new_name" "${base_name}-MacOS-${architecture}.zip"
    done

fi

cd "$initial_directory"
