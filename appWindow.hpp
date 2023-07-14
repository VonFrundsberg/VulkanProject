#pragma once

#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#include <string>

class appWindow {

public: 
	appWindow(uint32_t width, uint32_t height, std::string windowName);
	~appWindow();

	appWindow(const appWindow&) = delete;
	appWindow& operator=(const appWindow&);

	bool shouldClose() { return glfwWindowShouldClose(Window); }
	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
private:
	void initWindow();
	const uint32_t width;
	const uint32_t height;

	std::string WindowName;
	GLFWwindow* Window;
};