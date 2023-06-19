#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <map>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <fstream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) 
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else 
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) 
	{
		func(instance, debugMessenger, pAllocator);
	}
}

class HelloTriangleApplication 
{
public:

	void run() 
	{
		this->initWindow();
		this->initVulkan();
		this->mainLoop();
		this->cleanup();
	}

	void printPhysicalDeviceProperties()
	{
		using namespace std;
		if (this->physicalDevice == VK_NULL_HANDLE)
		{
			cout << "Physical device not initialized yet.";
			return;
		}
		
		VkPhysicalDeviceProperties deviceProperties = {};
		vkGetPhysicalDeviceProperties(this->physicalDevice, &deviceProperties);

		cout << "Device Name: " << deviceProperties.deviceName << endl;
		cout << "Device Type: " << deviceProperties.deviceType << endl;
		cout << "Device ID: " << deviceProperties.deviceID << endl;
		cout << "Driver Version: " << deviceProperties.driverVersion << endl;
		cout << "Api Version: " << deviceProperties.apiVersion << endl;
		cout << "Vendor ID: " << deviceProperties.vendorID << endl;
		cout << "Pipeline Cache UUID: " << deviceProperties.pipelineCacheUUID << endl;
		cout << "Device Limits:" << endl;
		cout << "\t" << "bufferImageGranularity: " << deviceProperties.limits.bufferImageGranularity << endl;
		cout << "\t" << "discreteQueuePriorities: " << deviceProperties.limits.discreteQueuePriorities << endl;
		cout << "\t" << "framebufferColorSampleCounts: " << deviceProperties.limits.framebufferColorSampleCounts << endl;
		cout << "\t" << "framebufferDepthSampleCounts: " << deviceProperties.limits.framebufferDepthSampleCounts << endl;
		cout << "\t" << "framebufferNoAttachmentsSampleCounts: " << deviceProperties.limits.framebufferNoAttachmentsSampleCounts << endl;
		cout << "\t" << "framebufferStencilSampleCounts: " << deviceProperties.limits.framebufferStencilSampleCounts << endl;
		cout << "\t" << "lineWidthGranularity: " << deviceProperties.limits.lineWidthGranularity << endl;
		cout << "\t" << "lineWidthRange: " << deviceProperties.limits.lineWidthRange << endl;
		cout << "\t" << "maxBoundDescriptorSets: " << deviceProperties.limits.maxBoundDescriptorSets << endl;
		cout << "\t" << "maxClipDistances: " << deviceProperties.limits.maxClipDistances << endl;
		cout << "\t" << "maxColorAttachments: " << deviceProperties.limits.maxColorAttachments << endl;
		cout << "\t" << "maxCombinedClipAndCullDistances: " << deviceProperties.limits.maxCombinedClipAndCullDistances << endl;
		cout << "\t" << "maxComputeSharedMemorySize: " << deviceProperties.limits.maxComputeSharedMemorySize << endl;
		cout << "\t" << "maxComputeWorkGroupCount: " << deviceProperties.limits.maxComputeWorkGroupCount << endl;
		cout << "\t" << "maxComputeWorkGroupInvocations: " << deviceProperties.limits.maxComputeWorkGroupInvocations << endl;
		cout << "\t" << "maxComputeWorkGroupSize: " << deviceProperties.limits.maxComputeWorkGroupSize << endl;
		cout << "\t" << "maxCullDistances: " << deviceProperties.limits.maxCullDistances << endl;
		cout << "\t" << "maxDescriptorSetInputAttachments: " << deviceProperties.limits.maxDescriptorSetInputAttachments << endl;
		cout << "\t" << "maxDescriptorSetSampledImages: " << deviceProperties.limits.maxDescriptorSetSampledImages << endl;
		cout << "\t" << "maxDescriptorSetSamplers: " << deviceProperties.limits.maxDescriptorSetSamplers << endl;
		cout << "\t" << "maxDescriptorSetStorageBuffers: " << deviceProperties.limits.maxDescriptorSetStorageBuffers << endl;
		cout << "\t" << "maxDescriptorSetStorageBuffersDynamic: " << deviceProperties.limits.maxDescriptorSetStorageBuffersDynamic << endl;
		cout << "\t" << "maxDescriptorSetStorageImages: " << deviceProperties.limits.maxDescriptorSetStorageImages << endl;
		cout << "\t" << "maxDescriptorSetUniformBuffers: " << deviceProperties.limits.maxDescriptorSetUniformBuffers << endl;
		cout << "\t" << "maxDescriptorSetUniformBuffersDynamic: " << deviceProperties.limits.maxDescriptorSetUniformBuffersDynamic << endl;
		cout << "\t" << "maxDrawIndexedIndexValue: " << deviceProperties.limits.maxDrawIndexedIndexValue << endl;
		cout << "\t" << "maxDrawIndirectCount: " << deviceProperties.limits.maxDrawIndirectCount << endl;
		cout << "\t" << "maxFragmentCombinedOutputResources: " << deviceProperties.limits.maxFragmentCombinedOutputResources << endl;
		cout << "\t" << "maxFragmentDualSrcAttachments: " << deviceProperties.limits.maxFragmentDualSrcAttachments << endl;
		cout << "\t" << "maxFragmentInputComponents: " << deviceProperties.limits.maxFragmentInputComponents << endl;
		cout << "\t" << "maxFragmentOutputAttachments: " << deviceProperties.limits.maxFragmentOutputAttachments << endl;
		cout << "\t" << "maxFramebufferHeight: " << deviceProperties.limits.maxFramebufferHeight << endl;
		cout << "\t" << "maxFramebufferLayers: " << deviceProperties.limits.maxFramebufferLayers << endl;
		cout << "\t" << "maxFramebufferWidth: " << deviceProperties.limits.maxFramebufferWidth << endl;
		cout << "\t" << "maxGeometryInputComponents: " << deviceProperties.limits.maxGeometryInputComponents << endl;
		cout << "\t" << "maxGeometryOutputComponents: " << deviceProperties.limits.maxGeometryOutputComponents << endl;
		cout << "\t" << "maxGeometryOutputVertices: " << deviceProperties.limits.maxGeometryOutputVertices << endl;
		cout << "\t" << "maxGeometryShaderInvocations: " << deviceProperties.limits.maxGeometryShaderInvocations << endl;
		cout << "\t" << "maxGeometryTotalOutputComponents: " << deviceProperties.limits.maxGeometryTotalOutputComponents << endl;
		cout << "\t" << "maxImageArrayLayers: " << deviceProperties.limits.maxImageArrayLayers << endl;
		cout << "\t" << "maxImageDimension1D: " << deviceProperties.limits.maxImageDimension1D << endl;
		cout << "\t" << "maxImageDimension2D: " << deviceProperties.limits.maxImageDimension2D << endl;
		cout << "\t" << "maxImageDimension3D: " << deviceProperties.limits.maxImageDimension3D << endl;
		cout << "\t" << "maxImageDimensionCube: " << deviceProperties.limits.maxImageDimensionCube << endl;
		cout << "\t" << "maxInterpolationOffset: " << deviceProperties.limits.maxInterpolationOffset << endl;
		cout << "\t" << "maxMemoryAllocationCount: " << deviceProperties.limits.maxMemoryAllocationCount << endl;
		cout << "\t" << "maxPerStageDescriptorInputAttachments: " << deviceProperties.limits.maxPerStageDescriptorInputAttachments << endl;
		cout << "\t" << "maxPerStageDescriptorSampledImages: " << deviceProperties.limits.maxPerStageDescriptorSampledImages << endl;
		cout << "\t" << "maxPerStageDescriptorSamplers: " << deviceProperties.limits.maxPerStageDescriptorSamplers << endl;
		cout << "\t" << "maxPerStageDescriptorStorageBuffers: " << deviceProperties.limits.maxPerStageDescriptorStorageBuffers << endl;
		cout << "\t" << "maxPerStageDescriptorStorageImages: " << deviceProperties.limits.maxPerStageDescriptorStorageImages << endl;
		cout << "\t" << "maxPerStageDescriptorUniformBuffers: " << deviceProperties.limits.maxPerStageDescriptorUniformBuffers << endl;
		cout << "\t" << "maxPerStageResources: " << deviceProperties.limits.maxPerStageResources << endl;
		cout << "\t" << "maxPushConstantsSize: " << deviceProperties.limits.maxPushConstantsSize << endl;
		cout << "\t" << "maxSampleMaskWords: " << deviceProperties.limits.maxSampleMaskWords << endl;
		cout << "\t" << "maxSamplerAllocationCount: " << deviceProperties.limits.maxSamplerAllocationCount << endl;
		cout << "\t" << "maxSamplerAnisotropy: " << deviceProperties.limits.maxSamplerAnisotropy << endl;
		cout << "\t" << "maxSamplerLodBias: " << deviceProperties.limits.maxSamplerLodBias << endl;
		cout << "\t" << "maxStorageBufferRange: " << deviceProperties.limits.maxStorageBufferRange << endl;
		cout << "\t" << "maxTessellationControlPerPatchOutputComponents: " << deviceProperties.limits.maxTessellationControlPerPatchOutputComponents << endl;
		cout << "\t" << "maxTessellationControlPerVertexInputComponents: " << deviceProperties.limits.maxTessellationControlPerVertexInputComponents << endl;
		cout << "\t" << "maxTessellationControlPerVertexOutputComponents: " << deviceProperties.limits.maxTessellationControlPerVertexOutputComponents << endl;
		cout << "\t" << "maxTessellationControlTotalOutputComponents: " << deviceProperties.limits.maxTessellationControlTotalOutputComponents << endl;
		cout << "\t" << "maxTessellationEvaluationInputComponents: " << deviceProperties.limits.maxTessellationEvaluationInputComponents << endl;
		cout << "\t" << "maxTessellationEvaluationOutputComponents: " << deviceProperties.limits.maxTessellationEvaluationOutputComponents << endl;
		cout << "\t" << "maxTessellationGenerationLevel: " << deviceProperties.limits.maxTessellationGenerationLevel << endl;
		cout << "\t" << "maxTessellationPatchSize: " << deviceProperties.limits.maxTessellationPatchSize << endl;
		cout << "\t" << "maxTexelBufferElements: " << deviceProperties.limits.maxTexelBufferElements << endl;
		cout << "\t" << "maxTexelGatherOffset: " << deviceProperties.limits.maxTexelGatherOffset << endl;
		cout << "\t" << "maxTexelOffset: " << deviceProperties.limits.maxTexelOffset << endl;
		cout << "\t" << "maxUniformBufferRange: " << deviceProperties.limits.maxUniformBufferRange << endl;
		cout << "\t" << "maxVertexInputAttributeOffset: " << deviceProperties.limits.maxVertexInputAttributeOffset << endl;
		cout << "\t" << "maxVertexInputAttributes: " << deviceProperties.limits.maxVertexInputAttributes << endl;
		cout << "\t" << "maxVertexInputBindings: " << deviceProperties.limits.maxVertexInputBindings << endl;
		cout << "\t" << "maxVertexInputBindingStride: " << deviceProperties.limits.maxVertexInputBindingStride << endl;
		cout << "\t" << "maxVertexOutputComponents: " << deviceProperties.limits.maxVertexOutputComponents << endl;
		cout << "\t" << "maxViewportDimensions: " << deviceProperties.limits.maxViewportDimensions << endl;
		cout << "\t" << "maxViewports: " << deviceProperties.limits.maxViewports << endl;
		cout << "Device Sparse Properties:" << endl;
		cout << "\t" << "residencyAlignedMipSize: " << (deviceProperties.sparseProperties.residencyAlignedMipSize ? "true" : "false") << endl;
		cout << "\t" << "residencyNonResidentStrict: " << (deviceProperties.sparseProperties.residencyNonResidentStrict ? "true" : "false") << endl;
		cout << "\t" << "residencyStandard2DBlockShape: " << (deviceProperties.sparseProperties.residencyStandard2DBlockShape ? "true" : "false") << endl;
		cout << "\t" << "residencyStandard2DMultisampleBlockShape: " << (deviceProperties.sparseProperties.residencyStandard2DMultisampleBlockShape ? "true" : "false") << endl;
		cout << "\t" << "residencyStandard3DBlockShape: " << (deviceProperties.sparseProperties.residencyStandard3DBlockShape ? "true" : "false") << endl;
	}

private:

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	GLFWwindow* window = nullptr;
	VkInstance instance = nullptr;
	VkDebugUtilsMessengerEXT debugMessenger = nullptr;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice = nullptr;
	VkQueue graphicsQueue = nullptr;
	VkSurfaceKHR surface = nullptr;
	VkQueue presentQueue = nullptr;
	VkSwapchainKHR swapChain = nullptr;
	std::vector<VkImage> swapChainImages{};
	VkFormat swapChainImageFormat{};
	VkExtent2D swapChainExtent{};
	std::vector<VkImageView> swapChainImageViews{};
	VkRenderPass renderPass = nullptr;
	VkPipelineLayout pipelineLayout = nullptr;
	VkPipeline graphicsPipeline = nullptr;
	std::vector<VkFramebuffer> swapChainFramebuffers{};
	VkCommandPool commandPool = nullptr;
	VkCommandBuffer commandBuffer = nullptr;
	VkSemaphore imageAvailableSemaphore = nullptr;
	VkSemaphore renderFinishedSemaphore = nullptr;
	VkFence inFlightFence = nullptr;

	void initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Don't create an OpenGL context.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // Window is not resizable.

		this->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void initVulkan() 
	{
		this->createInstance();
		this->setupDebugMessenger();
		this->createSurface();
		this->pickPhysicalDevice();
		this->createLogicalDevice();
		this->createSwapChain();
		this->createImageViews();
		this->createRenderPass();
		this->createGraphicsPipeline();
		this->createFrameBuffers();
		this->createCommandPool();
		this->createCommandBuffer();
		this->createSyncObjects();
	}

	void createSurface()
	{
		if (glfwCreateWindowSurface(this->instance, this->window, nullptr, &this->surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface!");
		}
	}

	void createLogicalDevice()
	{
		QueueFamilyIndices indicies = findQueueFamilies(this->physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indicies.graphicsFamily.value(), indicies.presentFamily.value() };
		float queuePriority = 1.0f;

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}
		
		if (vkCreateDevice(this->physicalDevice, &createInfo, nullptr, &this->logicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create logical device!");
		}

		vkGetDeviceQueue(this->logicalDevice, indicies.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(this->logicalDevice, indicies.presentFamily.value(), 0, &presentQueue);
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(this->window))
		{
			glfwPollEvents();
			this->drawFrame();
		}

		vkDeviceWaitIdle(this->logicalDevice);
	}

	void cleanup()
	{
		vkDestroySemaphore(this->logicalDevice, this->imageAvailableSemaphore, nullptr);

		vkDestroySemaphore(this->logicalDevice, this->renderFinishedSemaphore, nullptr);
		
		vkDestroyFence(this->logicalDevice, this->inFlightFence, nullptr);

		vkDestroyCommandPool(this->logicalDevice, this->commandPool, nullptr);

		for (auto framebuffer : this->swapChainFramebuffers) 
		{
			vkDestroyFramebuffer(this->logicalDevice, framebuffer, nullptr);
		}

		vkDestroyPipeline(this->logicalDevice, this->graphicsPipeline, nullptr);

		vkDestroyPipelineLayout(this->logicalDevice, this->pipelineLayout, nullptr);

		vkDestroyRenderPass(this->logicalDevice, this->renderPass, nullptr);

		for (auto imageView : this->swapChainImageViews)
		{
			vkDestroyImageView(this->logicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(this->logicalDevice, this->swapChain, nullptr);

		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(this->instance, this->debugMessenger, nullptr);
		}

		vkDestroyDevice(this->logicalDevice, nullptr);

		vkDestroySurfaceKHR(this->instance, this->surface, nullptr);

		vkDestroyInstance(this->instance, nullptr);

		glfwDestroyWindow(this->window);

		glfwTerminate();
	}

	void createInstance()
	{
		if (enableValidationLayers && !this->checkValidationLayerSupport()) 
		{
			throw std::runtime_error("validation layers requested, but not available!");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers) 
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			this->populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else 
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan instance!");
		}
	}

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) 
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	void setupDebugMessenger()
	{
		if (!enableValidationLayers)
		{
			return;
		}

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		this->populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(this->instance, &createInfo, nullptr, &this->debugMessenger) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to set up debug messenger!");
		}
	}

	std::vector<const char*> getRequiredExtensions() 
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) 
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) 
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

	void pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			throw std::runtime_error("Failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(this->instance, &deviceCount, devices.data());

		// Use an ordered map to automatically sort candidates by increasing score.
		std::multimap<int, VkPhysicalDevice> candidates;

		for (const auto& device : devices)
		{
			int score = this->rateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}

		// Check if the best candidate is suitable at all.
		if (candidates.rbegin()->first > 0)
		{
			this->physicalDevice = candidates.rbegin()->second;
		}
		else
		{
			throw std::runtime_error("Failed to find a suitable GPU!");
		}
	}

	int rateDeviceSuitability(VkPhysicalDevice device)
	{
		if (device == VK_NULL_HANDLE)
		{
			return 0;
		}

		VkPhysicalDeviceProperties deviceProperties = {};
		VkPhysicalDeviceFeatures deviceFeatures = {};
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		int score = 0;

		// Discrete GPUs have a significant performance advantage.
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;
		}

		// Maximum possible size of textures affects graphics quality.
		score += deviceProperties.limits.maxImageDimension2D;

		// Application can't function without geometry shaders.
		if (!deviceFeatures.geometryShader)
		{
			score = 0;
		}

		return score;
	}

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
	{
		if (device == VK_NULL_HANDLE)
		{
			return {};
		}

		QueueFamilyIndices indicies = {};

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indicies.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->surface, &presentSupport);
			if (presentSupport)
			{
				indicies.presentFamily = i;
			}

			if (indicies.isComplete())
			{
				break;
			}

			i++;
		}

		return indicies;
	}

	bool isDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = this->findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);
		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = querySwapChainSupportDetails(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	bool checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	SwapChainSupportDetails querySwapChainSupportDetails(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, this->surface, &details.capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			int width = 0;
			int height = 0;
			glfwGetFramebufferSize(this->window, &width, &height);

			VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupportDetails(this->physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = this->surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(this->physicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(this->logicalDevice, &createInfo, nullptr, &this->swapChain) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(this->logicalDevice, this->swapChain, &imageCount, nullptr);
		this->swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(this->logicalDevice, this->swapChain, &imageCount, this->swapChainImages.data());
		this->swapChainImageFormat = surfaceFormat.format;
		this->swapChainExtent = extent;
	}

	void createImageViews()
	{
		this->swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < this->swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = this->swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = this->swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(this->logicalDevice, &createInfo, nullptr, &this->swapChainImageViews[i]) != VK_SUCCESS) 
			{
				throw std::runtime_error("Failed to create image views!");
			}
		}
	}

	void createGraphicsPipeline()
	{
		auto vertShaderCode = this->readFile("src/shaders/vert.spv");
		auto fragShaderCode = this->readFile("src/shaders/frag.spv");

		VkShaderModule vertShaderModule = this->createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = this->createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)this->swapChainExtent.width;
		viewport.height = (float)this->swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = swapChainExtent;

		std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		if (vkCreatePipelineLayout(this->logicalDevice, &pipelineLayoutInfo, nullptr, &this->pipelineLayout) != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to create pipeline layout!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = this->pipelineLayout;
		pipelineInfo.renderPass = this->renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(this->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->graphicsPipeline) != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to create graphics pipeline!");
		}

		vkDestroyShaderModule(this->logicalDevice, vertShaderModule, nullptr);
		vkDestroyShaderModule(this->logicalDevice, fragShaderModule, nullptr);
	}

	void createRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = this->swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(this->logicalDevice, &renderPassInfo, nullptr, &this->renderPass) != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to create render pass!");
		}
	}

	void createFrameBuffers()
	{
		this->swapChainFramebuffers.resize(this->swapChainImageViews.size());

		for (size_t i = 0; i < this->swapChainImageViews.size(); i++) 
		{
			VkImageView attachments[] = { this->swapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = this->renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = this->swapChainExtent.width;
			framebufferInfo.height = this->swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(this->logicalDevice, &framebufferInfo, nullptr, &this->swapChainFramebuffers[i]) != VK_SUCCESS) 
			{
				throw std::runtime_error("Failed to create framebuffer!");
			}
		}
	}

	void createCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(this->physicalDevice);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		if (vkCreateCommandPool(this->logicalDevice, &poolInfo, nullptr, &this->commandPool) != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to create command pool!");
		}
	}

	void createCommandBuffer()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(this->logicalDevice, &allocInfo, &this->commandBuffer) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = this->renderPass;
		renderPassInfo.framebuffer = this->swapChainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = this->swapChainExtent;
		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsPipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(this->swapChainExtent.width);
		viewport.height = static_cast<float>(this->swapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = this->swapChainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to record command buffer!");
		}
	}

	void drawFrame()
	{
		vkWaitForFences(this->logicalDevice, 1, &this->inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(this->logicalDevice, 1, &this->inFlightFence);

		uint32_t imageIndex = 0;
		vkAcquireNextImageKHR(this->logicalDevice, this->swapChain, UINT64_MAX, this->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		vkResetCommandBuffer(this->commandBuffer, 0);
		recordCommandBuffer(this->commandBuffer, imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { this->imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &this->commandBuffer;
		VkSemaphore signalSemaphores[] = { this->renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(this->graphicsQueue, 1, &submitInfo, this->inFlightFence) != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { this->swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		vkQueuePresentKHR(this->presentQueue, &presentInfo);
	}

	void createSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateSemaphore(this->logicalDevice, &semaphoreInfo, nullptr, &this->imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(this->logicalDevice, &semaphoreInfo, nullptr, &this->renderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(this->logicalDevice, &fenceInfo, nullptr, &this->inFlightFence) != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to create semaphores!");
		}
	}

	VkShaderModule createShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(this->logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module!");
		}

		return shaderModule;
	}

	static std::vector<char> readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};

int main() 
{
	HelloTriangleApplication app;

	try 
	{
		app.run();
	}
	catch (const std::exception& e) 
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Success" << std::endl;
	return EXIT_SUCCESS;
}