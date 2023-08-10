#include "Application.hpp"
#include "appCamera.hpp"
#include "simpleRenderSystem.hpp"
#include "keyboardController.hpp"
#include <iostream>
#include <array>
#include <chrono>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace appNamespace {

	void Application::run()
	{
		SimpleRenderSystem simpleRenderSystem{ appDevice, appRenderer.getSwapChainRenderPass() };
        AppCamera camera{};
        //camera.setViewDirection(glm::vec3{ 0.0f }, glm::vec3{ 0.5f, 0.0f, 1.0f });
        camera.setViewTarget(glm::vec3{ -1.0f, -2.0f, 2.0f }, glm::vec3{0.0f, 0.0f, 2.5f});

        auto viewerObject = AppObject::createAppObject();
        KeyboardController cameraController{};
        auto currentTime = std::chrono::high_resolution_clock::now();


		while (!appWindow.shouldClose()) {
			glfwPollEvents();
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(appWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
            float aspect = appRenderer.getAspectRatio();
            //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(45.0f), aspect, 0.1f, 10.0f);

			if (auto commandBuffer = appRenderer.beginFrame()) {
				appRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderAppObjects(commandBuffer, this->appObjects, camera);
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
    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<AppModel> createCubeModel(AppDevice& device, glm::vec3 offset) {
        std::vector<AppModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<AppModel>(device, vertices);
    }

	void Application::loadObjects(){ 
        std::shared_ptr<AppModel> appModel = createCubeModel(appDevice, { 0.0f, 0.0f, 0.0f, });
        auto cube = AppObject::createAppObject();
        cube.model = appModel;
        cube.transform.translation = { 0.0f, 0.0f, 2.5f };
        cube.transform.scale = { 0.5f, 0.5f, 0.5f };

        appObjects.push_back(std::move(cube));
	}
    
};


