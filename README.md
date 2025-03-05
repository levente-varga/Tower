# Tower

#### Written in C++ and HLSL using Visual Studio

A game engine in development, powered by DirectX11.

Fully implemented in C++ and HLSL, features proper exception handling.

![Screenshot](images/Screenshot.png "Screenshot")

## Description

This is half a hobby project and half a university assignment. The ultimate goal is to implement an engine that could serve as the foundation of a simple video game.

In its current state, the engine is capable of rendering multiple cubes that randomly move around the scene using 3D transformations. Although keyboard and mouse input is also being captured, it is not used yet.

Error handling is crucial during development and testing, so a solution is implemented in the engine that upon encountering a graphics-related exception, displays an informative error message queried from the DirectX API.


## Roadmap

![Roadmap of the project](images/Roadmap.png "Roadmap")

- __First window__ - Display an empty window using the DirectX API.
- __Mouse and keyboard input__ - Maintain the full state of the keyboard and mouse keys.
- __Exception handling__ - Whenever the applications encounters an exception related to DirectX, a popup should be shown containing the description of the issue queried from the DirectX API.
- __Draw the first triangle__ - Draw a triangle to the window using shaders.
- __Transformations__ - Using matrices, move, rotate and scale the rendered meshed.
- __More efficient rendering__ - Reuse buffers if possible between frames.
- __Texturing__ - Set different colors for different faces of meshes. Could be extended to support actual textures being loaded from file.
- __Materials__ - Use a system for setting basic material properties for objects, such as shininess and color.
- __Directional and point lights__ - Shade the objects based on light coming from points (lamp) and set directions (sun), but without casting shadows.
- __Game objects__ - Every object in the game should derive from the same super class.
- __Camera system__ - The camera should be a game object with unique properties (transform, zoom, aspect ratio etc.). Support multiple cameras.
- __More primitive objects__ - Support a variety of built-in primitive objects like plane, cube and sphere. Would be useful for showcasing the capabilities of the engine.
