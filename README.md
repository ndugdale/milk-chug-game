<a name="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![GPLv3 License][license-shield]][license-url]

<div align="center">
<h1 align="center"><strong>Milk Chug</strong></h1>
  <p align="center">
    A simple game about chugging milk built in C.
    <br />
    <a href="https://github.com/ndugdale/milk-chug-game/issues">Report Bug</a>
    ·
    <a href="https://github.com/ndugdale/milk-chug-game/issues">Request Feature</a>
    ·
    <a href="#contact">Contact</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#download">Download</a></li>
    <li>
      <a href="#running-locally">Running Locally</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#build">Build and Run</a>a></li>
        <li><a href="#packaging">Packaging</a>a></li>
      </ul>
    </li>
    <li><a href="#licence">Licence</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
# About the Project
<p align="center">
  <img src="/docs/images/stage_0.png?raw=true" alt="Screenshot of Milk Chug game"/>
</p>

This is a simple 2D game about a milk chugging competition. This project was built in C using the [**SDL2**](https://www.libsdl.org/) library.

## Built With
[![C][c-language-shield]][c-language-url]
[![CMake][cmake-shield]][cmake-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- DOWNLOAD -->
# Download

Download a [Linux AppImage](https://github.com/ndugdale/milk-chug-game/releases), a [MacOS App](https://github.com/ndugdale/milk-chug-game/releases), or a [Windows Executable](https://github.com/ndugdale/milk-chug-game/releases).

<!-- RUNNING LOCALLY -->
# Running Locally

To get a local copy up and running follow the steps below:

## Prerequisites
### [**C**](https://github.com/canonical/lightdm](https://www.iso-9899.info/wiki/The_Standard))

You require a C compiler compatible with the C17 standard. On Windows, this project is configured to use the [MinGW](https://www.mingw-w64.org/) gcc compiler. If building on Windows, ensure a version of MinGW for your architecture is installed and set correctly on the `PATH` system variable.

### [**CMake**](https://cmake.org)

This project uses CMake as a build tool. You require a minimum version of 3.21. CMake can be downloaded from the [website](https://cmake.org) or through a package management tool on relevant OS distros. If on Windows, ensure CMake is set correctly on the `PATH` system variable.

E.g. on Arch-based distros:
```sh
sudo pacman -S cmake
```

And on MacOS:
```sh
brew install cmake
```

### [**SDL2**](https://www.libsdl.org)

Install [SDL2](https://github.com/libsdl-org/SDL/tree/SDL2) and the required extension libraries: [SDL2_image](https://github.com/libsdl-org/SDL_image/tree/SDL2), [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer/tree/SDL2), and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf/tree/SDL2)

- **Linux**: Download the libraries using the above links or your package manager of choice. E.g. on Arch-based distros:
    ```sh
    sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf
    ```

- **Mac**: Download the `.dmg` for each library and copy each respective `.framework` directory into `/Library/Frameworks`. Note that installation through homebrew will not provide the required `.framework` files.

- **Windows**: Download the `tar.gz` files and extract the library directory for your device's architecture (either `i686-w64-mingw32` or `x86_64-w64-mingw32`). Rename each corresponding folder to `SDL2`, `SDL2_image`, etc. for convenience and move them to the `C:/` drive where they can be found by the `CMakeLists.txt`.

## Installation

Clone the repository. Create a build directory if one does not already exist, and navigate inside it.
```sh
git clone https://github.com/ndugdale/milk-chug-game.git
mkdir -p milk-chug-game/build
cd milk-chug-game/build
```

<p align="right">(<a href="#readme-top">back to top</a>)</p> 

## Build and Run

Build using CMake. This will generate an executable named `MilkChug` in the `bin` directory.

```sh
cmake ..
cmake --build .
cmake --install . # only required on windows to install dll files to bin

cd ../bin
./MilkChug
```

## Packaging

Running the `build.sh` or `built.bat` scripts in the root directory will package the project differently according to the OS. The packaged app or installer will be located in the `install` directory.

On Linux or Mac:
```sh
./build.sh
```

On Windows:
```sh
.\build.bat
```

- **Linux**: `MilkChug-Linux-<ARCHITECTURE>.AppImage`
- **Mac**: `MilkChug-MacOS-<ARCHITECTURE>.app`
- **Windows**: `MilkChug-Windows-<ARCHITECTURE>.exe`



<!-- LICENCE -->
# Licence

Distributed under the GNU GPLv3 Licence. See `LICENCE.md` for more information.
<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
# Contact

Project Link: [https://github.com/ndugdale/milk-chug-game](https://github.com/ndugdale/milk-chug-game)
<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
# Acknowledgments
This project was only possible due to the works of others, and so I would like to extend a thank you to the following creators:
* Music: [Fupi](https://opengameart.org/content/dreamtune)
* Sound Effects: [thisusernameis](https://freesound.org/people/thisusernameis/sounds/426892/), [iwanPlays](https://freesound.org/people/iwanPlays/sounds/532749/)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/ndugdale/milk-chug-game.svg?style=for-the-badge
[contributors-url]: https://github.com/ndugdale/milk-chug-game/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/ndugdale/milk-chug-game.svg?style=for-the-badge
[forks-url]: https://github.com/ndugdale/milk-chug-game/network/members
[stars-shield]: https://img.shields.io/github/stars/ndugdale/milk-chug-game.svg?style=for-the-badge
[stars-url]: https://github.com/ndugdale/milk-chug-game/stargazers
[issues-shield]: https://img.shields.io/github/issues/ndugdale/milk-chug-game.svg?style=for-the-badge
[issues-url]: https://github.com/ndugdale/milk-chug-game/issues
[license-shield]: https://img.shields.io/github/license/ndugdale/milk-chug-game.svg?style=for-the-badge
[license-url]: https://github.com/ndugdale/milk-chug-game/blob/main/LICENCE.md
[c-language-shield]: https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white
[c-language-url]: https://www.iso-9899.info/wiki/The_Standard
[cmake-shield]: https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white
[cmake-url]: https://cmake.org
