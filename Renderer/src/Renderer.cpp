#include "Renderer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <stdexcept>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <iostream>

Renderer::Renderer()
{
	
}

void Renderer::init(const std::string& appName, const std::string& engineName, const std::vector<const char*>& requiredExtensions, const uint32_t width, const uint32_t height)
{
	VkExtent2D extents = {};
	extents.width = width;
	extents.height = height;

	this->createInstance(appName, engineName, requiredExtensions);
	this->pickPhysicalDevice();
}

Renderer::~Renderer()
{
	this->cleanup();
}

void Renderer::createInstance(const std::string appName, const std::string engineName, std::vector<const char*> requiredExtensions)
{
	if (enableValidationLayers && !checkValidationLayerSupport()) 
	{
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName.c_str();
	appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
	appInfo.pEngineName = engineName.c_str();
	appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();

	if (enableValidationLayers) 
	{
		createInfo.enabledLayerCount = (uint32_t)this->validationLayers.size();
		createInfo.ppEnabledLayerNames = this->validationLayers.data();
	}
	else 
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS) 
	{
		throw std::runtime_error("Failed to create instance!");
	}
}

void Renderer::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);

	if (deviceCount == 0) 
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices = {};
	vkEnumeratePhysicalDevices(this->instance, &deviceCount, devices.data());

	// If there's only one compatible device just pick it.
	if (deviceCount == 1)
	{
		this->physicalDevice = devices[0];
		return;
	}
}

void Renderer::cleanup()
{
	vkDestroyInstance(this->instance, nullptr);
}

bool Renderer::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : this->validationLayers) 
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) 
		{
			if (strcmp(layerName, layerProperties.layerName) == 0) 
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound) 
		{
			return false;
		}
	}

	return true;
}