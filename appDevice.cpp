#include "appDevice.hpp"
#include <iostream>

//void appDevice::createInstance()
//{
//	if (enableValidationLayers && !checkValidationLayerSupport()) {
//		throw std::runtime_error("validation layers requested, but not aviable");
//	}
//	//APP INFO
//	VkApplicationInfo appInfo{};
//	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//	appInfo.pApplicationName = "Hello triangle";
//	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//	appInfo.pEngineName = "No Engine";
//	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//	appInfo.apiVersion = VK_API_VERSION_1_0;
//
//	VkInstanceCreateInfo createInfo{};
//	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//	createInfo.pApplicationInfo = &appInfo;
//	auto extensions = getRequiredExtensions();
//	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
//	createInfo.ppEnabledExtensionNames = extensions.data();
//
//	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
//	if (enableValidationLayers) {
//		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//		createInfo.ppEnabledLayerNames = validationLayers.data();
//
//		populateDebugMessengerCreateInfo(debugCreateInfo);
//		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
//	}
//	else {
//		createInfo.enabledLayerCount = 0;
//
//		createInfo.pNext = nullptr;
//	}
//	VkResult createResult = vkCreateInstance(&createInfo, nullptr, &instance);
//	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
//		throw std::runtime_error("failed to create instance");
//	}
//}

appDevice::appDevice(appWindow& window)
{
}
