#pragma once

#include <string>

class Renderer;

class Application
{
public:

	Application(const std::string& appName, const std::string& engineName);
	void run();
	~Application();

private:

	Renderer* renderer = nullptr;

	std::string appName;
	std::string engineName;

	void initWindow();
	void initRenderer();
};