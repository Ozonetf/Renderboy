## A simple, cross-platform 3D renderer/engine with OpenGL
Simple 3D engine with OpenGL using [GLFW](https://www.glfw.org/) and [glad](https://github.com/dav1dde/glad), and other helper libraries such as [STB's image loader](https://github.com/nothings/stb).
## Developer Quick Start
```
git clone --recursive https://github.com/Ozonetf/Renderboy.git
cd Renderboy
cmake -B build
cmake --build build
./build/Renderboy
```
## Prerequisites
Renderboy is cross-platform, however, you will need [Git](https://git-scm.com/) and [Cmake](https://cmake.org/) to clone and build it. Check other prerequisites for your OS:
### Linux
On Debian and derivatives like Ubuntu and Linux Mint you will need the libwayland-dev and libxkbcommon-dev packages to compile for Wayland and the xorg-dev meta-package to compile for X11. These will pull in all other dependencies.
```
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
```
On Fedora and derivatives like Red Hat you will need the wayland-devel and libxkbcommon-devel packages to compile for Wayland and the libXcursor-devel, libXi-devel, libXinerama-devel and libXrandr-devel packages to compile for X11. These will pull in all other dependencies.
```
sudo dnf install wayland-devel libxkbcommon-devel libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel
```
On FreeBSD you will need the wayland, libxkbcommon and evdev-proto packages to compile for Wayland. The X11 headers are installed along the end-user X11 packages, so if you have an X server running you should have the headers as well. If not, install the xorgproto package to compile for X11.
```
pkg install wayland libxkbcommon evdev-proto xorgproto
```
On Cygwin Wayland is not supported but you will need the libXcursor-devel, libXi-devel, libXinerama-devel, libXrandr-devel and libXrender-devel packages to compile for X11. These can be found in the Libs section of the GUI installer and will pull in all other dependencies.

### Windows
You'll need a C++/C compiler, such as [Visual Studio](https://visualstudio.microsoft.com/downloads/) with C++ workload (Desktop Development with C++ in the installer). ALternaivly you can download [Winlibs](https://winlibs.com/) (recommended if you don't have visual studio), to install it run the following in the terminal:
```
winget install --id=BrechtSanders.WinLibs.POSIX.UCRT -e
```
If you are using Winlib, change the build command to:
```
cmake -B build -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build build
.\build\Renderboy
```
For Visual Studio:
```
cmake -B build
cmake --build build
.\build\Debug\Renderboy.exe
```
The executable is portable, just remember to also move all folders next to it.
### MacOs
In theory you should also be able to build and run on Mac, however Apple has deprecated OpenGL support so no guarantee on newer macs.
To be tested..

## Features 
* [x] Mouse+Keyboard Camera Control 
* [x] Phong Lighting model
* [x] Texture import
* [ ] Hot swapping shader(recompile and activate in runtime)
* [ ] PBR
* [ ] OBj support
* [ ] Dynamic Shadows
* [ ] Global Illumination
and more when I get more ideas
## Screenshots
Coming soon...
