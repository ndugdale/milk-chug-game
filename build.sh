#!/bin/sh

mkdir -p build
cd build

# Build project
cmake ..
cmake --build .

# Install project to AppDir for AppImage packaging
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

cd "$initial_directory"
