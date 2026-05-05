## A simple, cross-platform 3D renderer/engine with OpenGL
Simple 3D engine with OpenGL using [GLFW](https://www.glfw.org/) and [glad](https://github.com/dav1dde/glad), and other helper libraries such as [STB's image loader](https://github.com/nothings/stb).
## Cloning and building
Renderboy is cross-platform compatable, you will need [Git](https://git-scm.com/) and [Cmake](https://cmake.org/) installed to clone and build it. It can run 
on Windows, Linux and MacOs*. Follow the instruction for your OS:
## Linux
In a desired file location:
```
git clone --recursive https://github.com/Ozonetf/Renderboy.git
cd Renderboy
cmake -B build
cmake --build build
```
To run it:
```
./build/Renderboy
```
## Windows
You'll need a C++/C compiler, such as [Visual Studio](https://visualstudio.microsoft.com/downloads/) with C++ workload (Desktop Development with C++ in the installer). You can also use GCC or Clang if you know what you're doing, just remember to set the cmake compiler flag:
```
-DCMAKE_CXX_COMPILER={}
```
If you're comfortable with terminal the command are the same as the [Linux](#linux) section.
Otherwise, download [Github Desktop](https://desktop.github.com/download/) and clone the repository. Open terminal in the folder you saved the project in then:
```
cmake -B build
cmake --build build
```
To run it:
```
.\build\Debug\Renderboy.exe
```
or alternitvly click open Renderboy.exe, the app is portable too just ensure to also move all folders next to it.
## MacOs
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
