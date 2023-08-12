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

    struct GlobalUBO {
        glm::mat4 projectionView{ 1.0f };
        glm::vec3 lightDirection = glm::normalize(glm::vec3{ 1.0f, -3.0f, 1.0f });
    };


	void Application::run()
	{
        std::vector<std::unique_ptr<Buffer>> uboBuffers(AppSwapChain::MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<Buffer>(appDevice, sizeof(GlobalUBO),
                1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

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
            std::cout << 1.0 / abs(frameTimeFull) << std::endl;

            keyboardCameraController.moveInPlaneXZ(appWindow.getGLFWwindow(), frameTimeFull, viewerObject);
            mouseCameraController.moveInPlaneXZ(appWindow.getGLFWwindow(), frameTimeFull, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
            float aspect = appRenderer.getAspectRatio();
            
            camera.setPerspectiveProjection(glm::radians(45.0f), aspect, 0.1f, 1000.0f);

			if (auto commandBuffer = appRenderer.beginFrame()) {
                int frameIndex = appRenderer.getFrameIndex();
                FrameInfo frameInfo{frameIndex, frameTimeFull, commandBuffer, camera};
                //update 
                GlobalUBO ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                //render
				appRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderAppObjects(frameInfo, this->appObjects);
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

    void Application::loadObjects() {
        std::shared_ptr<AppModel> appModel = AppModel::createModelFromFile(appDevice, "./models/City1Block1.obj");
        int n = 1;
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                auto cube = AppObject::createAppObject();
                cube.model = appModel;
                cube.transform.translation = { 1 + 10 * i, 1, 1.0f + 10 * j };
                cube.transform.rotation = { 3.14 / 2, 0.0, 0.0f };
                cube.transform.scale = { 0.1f, 0.1f, 0.1f };

                appObjects.push_back(std::move(cube));
            }
        }
    }
        /*for (int i = 0; i < n; i++) {
            auto cube = AppObject::createAppObject();
            cube.model = appModel;
            cube.transform.translation = { 5 * sin(2 * 3.14 * i / n), 2.0f, 5 * cos(2 * 3.14 * i / n) };
            cube.transform.scale = { 0.5f, 0.5f, 0.5f };
            cube.transform.rotation = { 0.0f, -3.14f, -3.14f };

            appObjects.push_back(std::move(cube));
        }*/

        //appModel = AppModel::createModelFromFile(appDevice, "./models/tommy.obj");
        /*auto cube1 = AppObject::createAppObject();
        cube1.model = appModel;
        cube1.transform.translation = { 0.0f, 0.0f, 0.5f };
        cube1.transform.scale = { 0.5f, 0.5f, 0.5f };

        appObjects.push_back(std::move(cube1));*/
	
    
};


