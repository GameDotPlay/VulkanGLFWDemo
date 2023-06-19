# VulkanGLFWDemo
A demo of initializing Vulkan using GLFW as the windowing framework.
This demo is created by following the tutorial at [Vulkan Tutorial](https://vulkan-tutorial.com/) by Alexander Overvoorde.

![A triangle rendered using the Vulkan API and GLFW window.](docs/images/VulkanTriangle.png "The 1100 line triangle!")

## Dependencies
- [Vulkan SDK](https://vulkan.lunarg.com/)
- [GLFW 64-bit Windows Binaries](https://www.glfw.org/download.html)
- [GLM](https://github.com/g-truc/glm)

In the project properties, set the `C++->General->Additional Include Directories` and the `Linker->General->Additional Library Directories` accordingly:

![Set the project properties->C++->General->Additional Include Directories to point to the external dependencies.](docs/images/CPPGeneralIncludeDirectories.png "C++->General->Additional Include Directories")

![Set the project properties->Linker->General->Additional Library Directories to point to the external dependencies.](docs/images/LinkerGeneralLibDirectories.png "Linker->General->Additional Library Directories")
