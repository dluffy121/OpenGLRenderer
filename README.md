# OpenGL Mini Engine

A project to explore and understand the underlying mechanics of any game engine, like rendering objects, creating environments, lighting, etc. This project is based on OpenGL, one of the popular graphics library and hardware support for it is implemented widely in many graphical processing units. The project is written in C++ as it provides exceptional memory management, can have more control over resources making them flexibile and optimizable. The focus is on understanding concepts of OpenGL library like vertex and index buffers, their layouting using vertex buffer layouts, using Vertex Arrays, texture creation, etc. Along with this also explore different techniques of rendering in terms of world rendering using MVP matrices, using shaders to handle and use buffer data, architecturing entity component system, importing meshes, rendering GUIs, rendering different types of lights using Phong Lighting model and much more.

## Features

### **Windows**

Windows are created using GLFW library, which also takes care of creating OpenGL contexts, handling inputs and much more.<br>
A window can be created by calling ***WindowManager::GetWindowInstance*** from singleton instance of ***WindowManager***.

```cpp
    Window* window1 = WindowManager::getInstance()->GetWindowInstance("Hidden Window", 1, 1);
```

Multiple windows can exist at the same time. They can share resources or have resources of their own.
Resources like Vertex Buffers, Index buffers, Textures, Shaders can be shared across windows by passing GLFW window of an already created window.
Sharing resources helps in optimized performance and usage across multiple windows for different functionalities.

```cpp
	Window* window2 = WindowManager::getInstance()->GetWindowInstance("Second Window", 1280, 720, window1->GetGLFWWindow());
```

In terms of GUI, a font atlas object (***ImFontAtlas***) can be shared across windows.

```cpp
    ImGui::CreateContext();
	ImFontAtlas* globalFontAtlas = ImGui::GetIO().Fonts;
	Window* window3 = WindowManager::getInstance()->GetWindowInstance("Third Window", 1280, 720, window1->GetGLFWWindow(), globalFontAtlas);
```

### **GLSL Shaders**

Shaders are code that help program the stages of a render pipeline, for example:
1. **Vertex Shaders** which deal with processing of vertex
2. **Fragment shaders** which help push colors for pixels during rasterization
3. **Compute shaders** which help with complex calculations on arbitrary information.
4. and other shaders like Tessellation, Geometry

These code run on a graphical processing unit.<br>
In this project only 2 types of shaders are used **Vertex Shader** and **Fragment Shader** which are written in **GLSL** as shading language.<br>
These shader files can be found under ***[shaders](OpenGLRenderer/resources/shaders)*** folder:
* [Color](OpenGLRenderer/resources/shaders/Color.shader)
* [SimpleLit](resources/shaders/SimpleLit.shader)
* [SimpleUnlit](resources/shaders/SimpleUnlit.shader)
* [Texture](resources/shaders/Texture.shader)

In this project shaders can exist as Shader Assets. The reason for this is so every time a shader is requested the system does not need to parse and compile the shader file and just provide the shader.

### Shader Creation and Usage -
In order to use shader a shader asset needs to be created, this can be done as follows:
```cpp
ShaderManager::getInstance()->CreateShaderAsset("resources/shaders/Color.shader");
```
This shader can later be loaded when needed as follows:
```cpp
Shader* shader = ShaderManager::getInstance()->LoadShader("resources/shaders/Color.shader");
```
Unloading a shader as follow:
```cpp
ShaderManager::getInstance()->UnLoadShader(shader);
```

### **[Scene](OpenGLRenderer\src\Systems\Scene\Scene.h)**
A Scene is a 3D world canvas where objects live on. Scenes can be created by deriving from ***Scene*** class. Since scene editing is limited to code, any objects that needs to be in a scene needs to be declared inside the child class and initialized in the constructor.
In order to let Scenes window know about new scene, it must be register to the ***ScenesWindow*** instance of ***main*** method of ***OpenGLRenderer*** class as follows:

```cpp
// scenesWindow = already defined ScenesWindow instance
scenesWindow.RegisterScene<scene::NewScene>("NewScene");
```

### **[GameVastu](OpenGLRenderer\src\Systems\GameVastu\GameVastu.h)** (GameObject)

A GameVastu is a defining entity of an object. Any features of the object like transform and components is associated with its gameVastu instance. This helps identify an object as whole instead of separate features existing as separate entities to define a single object.<br>
A GameVastu has a base ***Transform*** component which can be used to translate, rotate, and scale any component in the scene.
A GameVastu can be created by calling ***scene::Scene::CreateGameVastu*** as follow:
```cpp
// written inside a scene constructor
GameVastu* cameraVastu = CreateGameVastu();
cameraVastu->m_name = "Camera";
```

### **[Component](OpenGLRenderer\src\Systems\Component\Component.h)**

A Component defines characteristics of a GameVastu. Muliple Components can be attached to a gameVastu. At a time only 1 component of same type can exist on a gameVastu, if trying to attach a new component of same type it won't be attached and will not exist. A component can be defined by deriving from ***Component*** class, or deriving from any other predefined component classes. Component creation can be done by simply calling its constructor.<br>
There are many virtual methods to override to achieve desired functionality:
1. Awake - called when the component is attached to a gameVastu and starts to exist.
2. Update - called each frame, to perform simple functionalities apart from rendering.
3. Render - called each frame for rendering objects.
4. OnEnable - called when component is enabled.
5. OnDisable - called when component is disabled.
6. OnGUI - called each frame, to render GUI related content.
7. OnInspectorGUI - called each frame, to render Inspector GUI related content.
8. OnDestroy - called when gameVastu or component cease to exist.

#### **List Of Components**

#### Camera
Camera Component is responsible for viewing and projecting the world on a 2D screen. It supports both types of projections, orthographic and perspective. This componont takes care of creating **View** matrix by using its gameVastu's Transform matrix and **Projection** matrix.
#### CameraController
This component is responsible for controlling cameras movement. **WASD** can be using to translate camera, rotation can be achieved by combining this with **Alt** key, and zooming can be done by using **Mouse Scroll**.
#### DirectionalLight
This component handles lighting objects from a single direction. Its direction and color can be handled from its inspector.
#### PointLight
This component handles lighting objects from a single point in the world. Its intensities, radius can be handled from its inspector.
#### SpotLight
This component handles lighting objects from a single point and spreading in a direction forming a conical light shape. Inspector can be used to modify its properties. This is a derived component of PointLight
#### Renderer
Rendering of simple models can be handled by this component by taking vertices and indices as input.
#### MeshRenderer
Rendering of complex ***obj*** models can be handled by this component. This is a derived component of Renderer.

Component creation and attaching to GameVastu:
```cpp
auto window = WindowManager::getInstance()->GetCurrentWindow();
Camera* camera = new Camera(window->GetWindowWidth(), window->GetWindowHeight());
cameraVastu->AddComponent(*camera);
```

### **Core**

The core part of this project contains scripts related to OpenGL library. Classes abstracting api usage:
1. **IndexBuffer**: Index buffers are OpenGL element array buffers which can store indices. This class manages lifetime of an index buffer, its binding, unbinding, and manipulation.
2. **VertexBuffer**: Vertex Buffers are OpenGL array buffers which can store vertices. This class manages lifetime of a vertex buffer, its binding, unbinding, and manipulation.
3. **VertexArray**: Vertex arrays are merely an array of references of buffer objects which can be enabled and disabled as per needs.This class manages lifetime of a vertex array, its binding, and unbinding.
4. **VertexBufferLayout**: Vertex Buffer Layouts are objects that help store layout of different element types like Vertex, Vector, primitive types, etc. This class manages lifetime of a vertex buffer layout object, its enabling and disabling.
5. **Texture**: Textures are responsible to render 2D images. This class manages lifetime of a texture, its binding, and unbinding.
6. **Material**: A Material is a collection of different colors, shaders, textures. This class manages lifetime of a material, its binding, and unbinding.

Other classes like
1. **Helper class**: Contains static methods related to GLFW and GLEW, initialization and helper methods.
2. **Logger**: Contains Macros to log messages for debugging
3. **Math**: Contains data types like Vectors, Vertex and some basic math functions.

### **GUI**

The GUI of the engine is handled using ImGUI library, which is a widely popular, robust and simple to use library. 
***[GUIWindow](OpenGLRenderer\src\Systems\GUI\GUIWindow.h)*** class can be derived from to create custom GUI windows. Currently the project has 4 predefined GUIWindows.
1. **Scenes Window**: Lists scenes available for loading and unloading.
2. **Heirarchy**: Window which shows loaded scenes and its gameVastus, which can be selected to inspect
3. **Inspector**: Interface to show information about selected GameVastu and its components. It also allows editing component exposed parameters. Some basic rendering information can also be inspected.
4. **Window Toolbar**: Comprises of window related functionalities like fullscreen toggling.

Custom GUIWindows mus override ***GUIWindow::Draw*** method and use the ImGUI library to draw GUIs.

Newly created GUI Windows must be registered as follows:
```cpp
InspectorWindow inspectorWindow;
// window = Window class object
window->RegisterGUIWindow(inspectorWindow);
```
To draw component related GUI in Inspector Window ***Component::OnInspectorGUI*** can be overidden.
```cpp
// CustomComponent derives from Component
void CustomComponent::OnInspectorGUI()
{
    // m_customVariable is a boolean variable of CustomComponent class
	bool paramUpdated = ImGui::Checkbox("Custom Param 1: ", &m_customVariable);
    if(paramUpdated)
        // Do something
}
```

### **[RenderIntent](OpenGLRenderer\src\Systems\RenderIntent\RenderIntent.h)**
In order to control the way rendering resources are used a RenderIntent can be created. This not only helps separate resource management logic from renderers but also help keep tabs on those resources from one place. So Renderers just need to request resources and not care about managing them. At present there are 2 RenderIntents:
1. Standard RenderIntent : Creates separate resources for each object and issues draw calls for each object.
2. Batch RenderIntent : Creates one large resource shared among all objects and only one draw call is issued.

## **Some Limitations**
1.  Batch Rendering only supports usage of only single Batch per material, which can cause not rendering of objects if their indices of vertices exceed batch's buffer limit.
2.  Batch Rendering cause some lit objects to not render properly.

## **Third Party Libraries**

1. [GLFW](https://www.glfw.org/) - Cross-platform OpenGL Library for context creation, window creation, and input events handling. 
2. [GLEW](http://glew.sourceforge.net/) - OpenGL core library for C/C++
3. [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) - Image Loading Library
4. [GLM](https://github.com/g-truc/glm) - Math Library
5. [Dear Imgui](https://github.com/ocornut/imgui) - GUI library
6. [ASSIMP](https://github.com/assimp/assimp) - Asset Import library