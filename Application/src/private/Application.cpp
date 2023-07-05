#include "../public/Application.h"
#include "../../../Renderer/src/public/Renderer.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <stdexcept>

Application::Application(const std::string& appName, const std::string& engineName, int width, int height)
{
	this->appName = appName;
	this->engineName = engineName;
	this->windowWidth = width;
	this->windowHeight = height;

	this->initWindow();
	this->initRenderer();
}

void Application::run()
{
	this->mainLoop();
}

Application::~Application()
{
	delete this->renderer;
	glfwTerminate();
}

void Application::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Don't create an OpenGL context.

	this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, this->appName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(this->window, this);
	glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallback);
}

void Application::initRenderer()
{
	this->initVulkan();
}

void Application::initVulkan()
{
	std::vector<const char*> extensions = this->getRequiredExtensions();
	this->renderer = new Renderer(this->appName, this->engineName, extensions);

	if (this->renderer->getInstance() == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Vulkan instance could not be created!");
	}
}

void Application::mainLoop()
{
	while (!glfwWindowShouldClose(this->window))
	{
		glfwPollEvents();
	}
}

std::vector<const char*> Application::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return extensions;
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	app->windowResized = true;
}
