# 3D-SCII   

A software 3D renderer that displays models directly in the terminal using ANSI escape codes and ASCII characters. Written in C, this project implements a basic graphics pipeline including mesh loading, lighting, and rasterization using just the standard library.

![3D-SCII](
    3d-scii.gif
)

## Features

- OBJ file loading and parsing
- Basic lighting system with ambient and point lights
- Wireframe and filled polygon rendering modes 
- Z-buffer for proper depth handling
- Terminal-based display using ANSI colors
- Frame buffering for smooth animation
- Vector/matrix math operations for 3D transformations

## Building

This project uses CMake for building. To build:

```sh
mkdir build
cd build
cmake ..
make
```

## Usage
Run the compiled executable:
```sh
./build
```

The program will load and display the 3D model specified in FILENAME.

- The point light source auto-rotates to demonstrate 3D rendering
- Press Ctrl+C to exit

## Configuration
Edit the following defines in main.c to change the model and scaling:
```c
#define FILENAME "Mask01.obj"     // OBJ file to load
#define SCALING_FACTOR 0.0004f    // Model scale factor
```

Display settings can be configured in setting.h:
```c
#define WIDTH 200                // Terminal width
#define HEIGHT 100                // Terminal height
```