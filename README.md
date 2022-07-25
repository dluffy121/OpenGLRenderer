# Basic OpenGL Renderer

A Learning project to not only learn about the OpenGL Graphics Library but also learn C++ Language

## Features

1. Multiple GLFW Windows
2. GLSL Shader programming
3. Texture Creation
4. Creating <b>GameVastu</b>s (GameObjects)
5. Attaching Components to it, like:<br>

   - Camera \- Orthographic and Perspective Camera support with near and far clipping planes
   - Renderer \- uses Vertex Coordinates, Color, Texture Coordinates, Texture Samplers, and Triangle Indices to render objects
   - Mesh Renderer \- uses obj and simple texture formats(jpg, png) to render meshes
   - Light \- Directional, Point and Spot lights based on Phong Lighting model can be added
<br>
<br>
6. Using Transforms to change Position, Rotation and Scale of GameVastus
7. Scene support to create and load multiple scenes
8. GUI Windows to easily inspect, modify, and load scenes (GUIWindows - Scenes, Inspector, Hierarchy)
9.  RenderIntent to request resources and manage buffers and draw calls as per performance needs.
    1. Standard RenderIntent : Creates separate resources for each object and issues draw calls for each object.
    2. Batch RenderIntent : Creates one large resource shared among all objects and only one draw call is issued.
<br>
<br>
11. Batch Rendering for multiple objects using same Material (Partial WIP)
    - Features:
        - Can Batch multiple objects using same material
    - Limitations:
        - only supports single Batch
        - doesn't work on Loaded Obj Models


<!-- ## Examples -->
<!-- Following are few examples of code samples to render things using OpenGL -->

## Third Party Libraries used

1. [GLFW](https://www.glfw.org/) - Cross-platform OpenGL Library for context creation, window creation, and input events handling. 
2. [GLEW](http://glew.sourceforge.net/) - OpenGL core library for C/C++
3. [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) - Image Loading Library
4. [GLM](https://github.com/g-truc/glm) - Math Library
5. [Dear Imgui](https://github.com/ocornut/imgui) - GUI library
6. [ASSIMP](https://github.com/assimp/assimp) - Asset Import library
