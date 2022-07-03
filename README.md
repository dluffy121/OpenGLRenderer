# Basic OpenGL Renderer

A Learning project to not only learn about the OpenGL Graphics Library but also learn C++ Language

## Features

1. Multiple GLFW Windows
2. GLSL Shader programming
3. Texture Creation
4. Creating <b>GameVastu</b>s (GameObjects)
5. Attaching Components to it, like

    - Camera \- Orthographic and Perspective Camera support with near and far clipping planes
    - Renderer \- uses Vertex Coordinates, Color, Texture Coordinates, Texture Samplers, and Triangle Indices to render objects
    - Mesh Renderer \- uses obj and simple texture formats(jpg, png) to render meshes
    - Light \- Basic Directional lights based on Phong Lighting model

6. Using Transforms to change Position, Rotation and Scale of GameVastus
7. Scene support to create and load multiple scenes
8. GUI Windows to easily inspect, modify, and load scenes (GUIWindows - Scenes, Inspector, Hierarchy)
<!-- 10. Batch Rendering for multiple objects using same Shader (Partial WIP - only supports single Batch) -->


<!-- ## Examples -->
<!-- Following are few examples of code samples to render things using OpenGL -->

## Third Party Libraries used

1. [GLFW](https://www.glfw.org/) - Cross-platform OpenGL Library for context creation, window creation, and input events handling. 
2. [GLEW](http://glew.sourceforge.net/) - OpenGL core library for C/C++
3. [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) - Image Loading Library
4. [GLM](https://github.com/g-truc/glm) - Math Library
5. [Dear Imgui](https://github.com/ocornut/imgui) - GUI library
