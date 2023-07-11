#pragma once

#define K_USE_PLATFORM_ANDROID_KHR
#include <vulkan/vulkan.h>

#include "QueueFamilyIndices.hpp"
#include <vector>
#include <string>


class Renderer
{
public:

	Renderer();
	~Renderer();
	void init(const std::string& appName, const std::string& engineName, const std::vector<const char*>& requiredExtensions, const uint32_t width, const uint32_t height);
	VkInstance getInstance() { return this->instance; }
	VkPhysicalDevice getPhysicalDevice() { return this->physicalDevice; }
	
private:

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	
	void createInstance(const std::string appName, const std::string engineName, std::vector<const char*> requiredExtensions);
	void pickPhysicalDevice();
	void createLogicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void cleanup();

	bool checkValidationLayerSupport();

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
};