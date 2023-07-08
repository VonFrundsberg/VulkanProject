#include "Application.h"

void Application::run()
{
	while (!Window.shouldClose()) {
		glfwPollEvents();
	}
}
