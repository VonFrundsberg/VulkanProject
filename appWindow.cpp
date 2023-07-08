#include "appWindow.hpp"
#include <iostream>


appWindow::appWindow(uint32_t width, uint32_t height, std::string windowName) : width{ width }, height{height}, WindowName{windowName}
{
	initWindow();
}

appWindow::~appWindow() {
	glfwDestroyWindow(Window);
	glfwTerminate();
}

void appWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR surface)
{
	if (glfwCreateWindowSurface(instance, Window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

void appWindow::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	Window = glfwCreateWindow(width, height, WindowName.c_str(), nullptr, nullptr);
}
