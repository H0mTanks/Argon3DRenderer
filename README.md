# Argon3DRenderer
Argon is a 3D Software Renderer written in C++ and using SDL2.
upng library was used to decode png images and get them into byte buffers.

Argon3D reads in a .obj file with an optional png texture and renders the 3D mesh on the screen.
The mesh can be rotated, translated and scaled through the WASD and Arrow Keys.


The different rendering modes supported are shown in the gif below

![](https://github.com/ActualHomTanks/gifs/blob/main/argon.gif)

## Usage
Currently the .obj filepath needs to be supplied in the App.cpp file.

### Options
1. Press 1 to render the vertices and the wireframe.
2. Press 2 to render the wireframe only.
3. Press 3 to render the flat shaded model.
4. Press 4 to render the flat shaded model with wireframe.
5. Press 5 to render the textured model.
6. Press 6 to render the textured model with wireframe.

The mesh can be rotated, translated and scaled through the WASD and Arrow Keys.

## Concepts learnt and applied in this project
- Vector and Matrix Transformations in 3D graphics
- Line Drawing and Rasterization algorithms
- Reading in OBJ files and using the data to render meshes
- Projection Matrices
- Light and Flat Shading
- Culling and clipping of polygons: Backface culling and frustum clipping
- Texture Mapping with PNG files
- Perspective Correct Interpolation
- Z-Buffer for hidden surface detection
