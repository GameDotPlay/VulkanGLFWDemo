#pragma once

#define K_USE_PLATFORM_ANDROID_KHR
#include <vulkan/vulkan.h>

#include <vector>
#include <string>

class Renderer
{
public:
	Renderer();
	void init(const std::string& appName, const std::string& engineName, const std::vector<const char*>& requiredExtensions, const uint32_t width, const uint32_t height);
	VkInstance getInstance() { return this->instance; }
	~Renderer();

private:

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkInstance instance = VK_NULL_HANDLE;
	
	void createInstance(const std::string appName, const std::string engineName, std::vector<const char*> requiredExtensions);
	void cleanup();

	bool checkValidationLayerSupport();
};