#include "Application.h"
namespace appNamespace {
	void Application::run()
	{
		while (!Window.shouldClose()) {
			glfwPollEvents();
		}
	}
};
