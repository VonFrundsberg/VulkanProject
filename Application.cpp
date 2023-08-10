#include "Application.hpp"
#include "simpleRenderSystem.hpp"
#include <iostream>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace appNamespace {

	void Application::run()
	{
		SimpleRenderSystem simpleRenderSystem{ appDevice, appRenderer.getSwapChainRenderPass() };
		while (!appWindow.shouldClose()) {
			glfwPollEvents();
			if (auto commandBuffer = appRenderer.beginFrame()) {
				appRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderAppObjects(commandBuffer, this->appObjects);
				appRenderer.endSwapChainRenderPass(commandBuffer);
				appRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(appDevice.device());
	}
	Application::Application()
	{	
		loadObjects();
	}
	Application::~Application(){
	}
	void Application::loadObjects(){
		std::vector<AppModel::Vertex> vertices{
			{{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
		};
		auto appModel = std::make_shared<AppModel>(appDevice, vertices);
		auto triangle = AppObject::createAppObject();
		triangle.model = appModel;
		triangle.color = { 0.1f, 0.8f, 0.1f, 0.0f };
		triangle.transform.translation.x = 0.2f;
		triangle.transform.scale.x = 1.2f;
		triangle.transform.rotation.z = .25f * glm::two_pi<float>();


		appObjects.push_back(std::move(triangle));
	}
};


