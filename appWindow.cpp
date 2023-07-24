#include "appWindow.hpp"
#include <iostream>

namespace appNamespace {
	AppWindow::AppWindow(int width, int height, std::string windowName) : width{ width }, height{ height }, WindowName{ windowName }
	{
		initWindow();
	}

	AppWindow::~AppWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void AppWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void AppWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto appWindow = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
		appWindow->framebufferResized = true;
		appWindow->width = width;
		appWindow->height = height;
	}

	void AppWindow::initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); 

		window = glfwCreateWindow(width, height, WindowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}
};