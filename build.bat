@echo off
setlocal

set initial_directory=%cd%
set script_directory=%~dp0
cd %script_directory%

if "%PROCESSOR_ARCHITECTURE%"=="x86" (
    set architecture=x86_32
) else if "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
    set architecture=x86_64
) else (
    set architecture=unknown
)

mkdir build
cd build

set PACKAGE_APP=1

cmake .. -G "MinGW Makefiles"
cmake --build .
cmake --install . --prefix .

set installer_name=MilkChug-Windows-%architecture%
echo "..\install\%installer_name%"
iscc ..\resources\windows\setup.iss /F"%installer_name%"

cd %initial_directory%

endlocal