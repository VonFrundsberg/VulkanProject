#pragma once
#include "appWindow.hpp"

// std lib headers
#include <string>
#include <vector>
namespace appNamespace {
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		uint32_t graphicsFamily;
		uint32_t presentFamily;
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class AppDevice {
	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		AppDevice(AppWindow& window);
		~AppDevice();
		// Not copyable or movable
		AppDevice(const AppDevice&) = delete;
		AppDevice& operator=(const AppDevice&) = delete;
		AppDevice(AppDevice&&) = delete;
		AppDevice& operator=(AppDevice&&) = delete;

		VkCommandPool getCommandPool() { return commandPool; }
		VkDevice device() { return device_; }
		VkSurfaceKHR surface() { return surface_; }
		VkQueue graphicsQueue() { return graphicsQueue_; }
		VkQueue presentQueue() { return presentQueue_; }
		VkSampler getTextureSampler() { return textureSampler_; }
		VkInstance getInstance() { return _instance; }
		VkPhysicalDevice getPhysicalDevice() { return _physicalDevice; }
		SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(_physicalDevice); }
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(_physicalDevice); }
		VkFormat findSupportedFormat(
			const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		// Buffer Helper Functions
		void createBuffer(
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory);
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void copyBufferToImage(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

		void createImageWithInfo(
			const VkImageCreateInfo& imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory);

		VkPhysicalDeviceProperties properties;

	private:
		VkDevice device_;
		void createInstance();
		void setupDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createCommandPool();
		void createTextureSampler();

		// helper functions
		bool isDeviceSuitable(VkPhysicalDevice device);
		std::vector<const char*> getRequiredExtensions();
		bool checkValidationLayerSupport();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void hasGflwRequiredInstanceExtensions();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		//void setDeviceExtensions();
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		VkInstance _instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		AppWindow& window;
		VkCommandPool commandPool;
		VkSampler textureSampler_;

		VkSurfaceKHR surface_;
		VkQueue graphicsQueue_;
		VkQueue presentQueue_;

		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> deviceExtensions = {
		#ifdef _WIN32
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		#else
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			"VK_KHR_portability_subset"
		#endif
		};
	};
};