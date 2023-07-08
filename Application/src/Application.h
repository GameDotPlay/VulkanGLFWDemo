#pragma once

#include <string>
#include <vector>

class Renderer;
struct GLFWwindow;

class Application
{
public:

	Application(const std::string& appName, const std::string& engineName, int width = 1280, int height = 720);
	void run();
	~Application();

private:

	GLFWwindow* window = nullptr;
	Renderer* renderer = nullptr;

	bool windowResized = false;
	int windowWidth = 1280;
	int windowHeight = 720;
	std::string appName;
	std::string engineName;

	void initWindow();
	void initRenderer();
	void mainLoop();

	std::vector<const char*> getRequiredExtensions();

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};