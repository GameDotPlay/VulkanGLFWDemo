#include "../public/Application.h"
#include "../../../Renderer/src/public/Renderer.h"
#include "SDL2/SDL_vulkan.h"

Application::Application(const std::string& appName, const std::string& engineName)
{
	this->initWindow();
	this->initRenderer();
}

void Application::run()
{

}

Application::~Application()
{

}

void Application::initWindow()
{

}

void Application::initRenderer()
{

}
