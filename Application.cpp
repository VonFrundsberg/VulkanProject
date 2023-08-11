#include "Application.hpp"
#include "src/appCamera.hpp"
#include "src/simpleRenderSystem.hpp"
#include "src/controller/keyboardController.hpp"
#include "src/controller/mouseController.hpp"


#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace appNamespace {

	void Application::run()
	{
		SimpleRenderSystem simpleRenderSystem{ appDevice, appRenderer.getSwapChainRenderPass() };
        AppCamera camera{};
        camera.setViewTarget(glm::vec3{ -1.0f, -2.0f, 2.0f }, glm::vec3{0.0f, 0.0f, 2.5f});

        auto viewerObject = AppObject::createAppObject();
        KeyboardController keyboardCameraController{};
        MouseController mouseCameraController{appWindow.getGLFWwindow(), true};
        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!appWindow.shouldClose()) {
			glfwPollEvents();
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTimeFull = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;
            frameTimeFull = glm::min(frameTimeFull, MAX_FRAME_TIME);
            //std::cout << 1.0 / abs(frameTimeFull) << std::endl;

            keyboardCameraController.moveInPlaneXZ(appWindow.getGLFWwindow(), frameTimeFull, viewerObject);
            mouseCameraController.moveInPlaneXZ(appWindow.getGLFWwindow(), frameTimeFull, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
            float aspect = appRenderer.getAspectRatio();
            
            camera.setPerspectiveProjection(glm::radians(45.0f), aspect, 0.1f, 10.0f);

			if (auto commandBuffer = appRenderer.beginFrame()) {
				appRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderAppObjects(commandBuffer, this->appObjects, camera);
				appRenderer.endSwapChainRenderPass(commandBuffer);
				appRenderer.endFrame();
			}
            auto renderingTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - std::chrono::high_resolution_clock::now()).count();
            std::this_thread::sleep_for(std::chrono::duration < float, std::chrono::seconds::period>(1.0 / this->FPS_CAP + renderingTime));
		}
		vkDeviceWaitIdle(appDevice.device());
	}
	Application::Application()
	{	
		loadObjects();
	}
	Application::~Application(){
	}
    //// temporary helper function, creates a 1x1x1 cube centered at offset
    //std::unique_ptr<AppModel> createCubeModel(AppDevice& device, glm::vec3 offset) {
    //    AppModel::Builder modelBuilder{};
    //    modelBuilder.vertices = {

    //        // left face (white)
    //        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    //        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
    //        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
    //        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    //        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
    //        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

    //        // right face (yellow)
    //        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    //        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
    //        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
    //        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    //        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
    //        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

    //        // top face (orange, remember y axis points down)
    //        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    //        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    //        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

    //        // bottom face (red)
    //        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
    //        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
    //        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

    //        // nose face (blue)
    //        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

    //        // tail face (green)
    //        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

    //    };
    //    for (auto& v : modelBuilder.vertices) {
    //        v.position += offset;
    //    }
    //    return std::make_unique<AppModel>(device, modelBuilder);
    //}
    // temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
    std::unique_ptr<AppModel> createCubeModel(AppDevice& device, glm::vec3 offset) {
        AppModel::Builder modelBuilder{};
        modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };
        for (auto& v : modelBuilder.vertices) {
            v.position += offset;
        }

        modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

        return std::make_unique<AppModel>(device, modelBuilder);
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


