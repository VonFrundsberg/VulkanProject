#include "Application.hpp"
#include "src/appCamera.hpp"
#include "src/renderSystems/simpleRenderSystem.hpp"
#include "src/renderSystems/UI_RenderSystem.hpp"
#include "src/controller/keyboardController.hpp"
#include "src/controller/mouseController.hpp"


#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include <unordered_map>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace appNamespace {

    struct GlobalUBO {
        alignas(16) glm::mat4 projectionView{ 1.0f };
        alignas(16) glm::vec3 directionToLight = glm::normalize(glm::vec3{ -1.0f, 1.0f, -1.0f });
    };


	void Application::run()
	{
        std::vector<std::unique_ptr<Buffer>> uboBuffers(AppSwapChain::MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<Buffer>(appDevice, sizeof(GlobalUBO),
                1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = DescriptorSetLayout::Builder(appDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
            .build();

        std::unordered_map<std::string, std::vector<VkDescriptorSet>> unorderedMapOfglobalDescriptorSets;
        for (auto& texture : this->_loadedTextures){
            unorderedMapOfglobalDescriptorSets.emplace(texture.first, std::vector<VkDescriptorSet>(2));
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = texture.second->getTextureImageView();
            imageInfo.sampler = appDevice.getTextureSampler();

            for (int j = 0; j < unorderedMapOfglobalDescriptorSets.at(texture.first).size(); j++) {
                auto bufferInfo = uboBuffers[j]->descriptorInfo();
                DescriptorWriter(*globalSetLayout, *this->globalPools.at(texture.first))
                    .writeBuffer(0, &bufferInfo)
                    .writeImage(1, &imageInfo)
                    .build(unorderedMapOfglobalDescriptorSets.at(texture.first)[j]);
            }
            texture.second->textureDescriptors = &unorderedMapOfglobalDescriptorSets.at(texture.first);
        }
        std::vector<VkDescriptorSet> globalDescriptorSets(AppSwapChain::MAX_FRAMES_IN_FLIGHT);
        

		SimpleRenderSystem simpleRenderSystem{ appDevice, appRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout() };
        /*UIRenderSystem UI_RenderSystem{ appDevice, appRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout() };*/
         
        AppCamera camera{};
        glm::vec3 cameraDistanceToPlayer = { 0.0f, -2.0f, -1.5f };
        //glm::vec3 cameraRotation = { -3.14f, -3.14f, 0.0f };
        glm::vec3 cameraRotation = { 0.0f, 0.0f, 0.0f };
        auto viewerObject = AppObject::createAppObject();
        //&appObjects[0].transform.translation = viewerObject.transform.translation;
        //viewerObject.transform.translation = appObjects[0].transform.translation;
        //viewerObject.transform.translation += cameraDistanceToPlayer;
        //viewerObject.transform.rotation = cameraRotation;
        //AppObject * viewerObject = &appObjects[0];
        KeyboardController keyboardCameraController{};
        MouseController mouseCameraController{appWindow.getGLFWwindow(), true};
        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!appWindow.shouldClose()) {
			glfwPollEvents();
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTimeFull = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;
            //frameTimeFull = glm::min(frameTimeFull, MAX_FRAME_TIME);
            //std::cout << 1.0 / abs(frameTimeFull) << std::endl;
            //std::cout << "total time passed: " << abs(frameTimeFull) << std::endl;
            auto dt = glm::min(frameTimeFull, MAX_FRAME_TIME);
            keyboardCameraController.moveInPlaneXZ(appWindow.getGLFWwindow(), dt, viewerObject);
            mouseCameraController.moveInPlaneXZ(appWindow.getGLFWwindow(), dt, viewerObject);
            //camera.setViewYXZ((viewerObject).transform.translation, (viewerObject).transform.rotation);
            camera.set3rdPersonCameraView(
                cameraDistanceToPlayer, (viewerObject).transform.translation,
                (viewerObject).transform.rotation);
            appObjects[0].transform.translation = viewerObject.transform.translation;
            appObjects[0].transform.rotation.y = 3.14 + viewerObject.transform.rotation.y;

            float aspect = appRenderer.getAspectRatio();
            
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.01f, 1000.0f);

			if (auto commandBuffer = appRenderer.beginFrame()) {
                int frameIndex = appRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    dt,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex], 
                    appObjects};
                //update 
                GlobalUBO ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                //render
				appRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderAppObjects(frameInfo);
                //UI_RenderSystem.render(frameInfo);
				appRenderer.endSwapChainRenderPass(commandBuffer);
				appRenderer.endFrame();
			}
            auto renderingTime = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - newTime).count();
            //std::cout << "sleep for microseconds: " << int(1000000 * (1.0 / this->FPS_CAP - renderingTime)) << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(int( 1000000*(1.0/ this->FPS_CAP - renderingTime))));
            //std::cout << "should be: " << 1.0 / this->FPS_CAP << ", but we have: " << renderingTime << std::endl;
           // std::cout << "before sleep we have: " << renderingTime << std::endl;
		}
		vkDeviceWaitIdle(appDevice.device());
	}
	Application::Application()
	{   
        /*globalPool = DescriptorPool::Builder(appDevice)
            .setMaxSets(AppSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, AppSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, AppSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
        globalPool1 = DescriptorPool::Builder(appDevice)
            .setMaxSets(AppSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, AppSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, AppSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();*/
		
        loadTextures();
        loadObjects();
        for (auto &texture: this->_loadedTextures) {
            this->globalPools.emplace(texture.first, DescriptorPool::Builder(appDevice)
                .setMaxSets(AppSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, AppSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, AppSwapChain::MAX_FRAMES_IN_FLIGHT)
                .build());
        }
	}
	Application::~Application(){

	}  

    void Application::loadObjects() {
        
        int n = 1;
        std::shared_ptr<AppModel> appModel1 = AppModel::createModelFromFile(appDevice, "./models/tommy.obj");
        for (int j = 0; j < 1; j++) {
            for (int i = 0; i < 1; i++) {
                auto player = AppObject::createAppObject();
                player.model = appModel1;
                player.texture = this->_loadedTextures["tommy"];
                player.transform.translation = { 0, 0.0, 0 };
                player.transform.rotation = { 3.14, 3.14f , 0.0};
                //house.transform.rotation = { 3.14, 0.0, 0.0f };
                //house.transform.scale = { 0.5f, 0.5f, 0.5f };
                player.transform.scale = { 0.5f, 0.5f, 0.5f };
                appObjects.emplace(player.getId(), std::move(player));
            }
        }
        n = 10;
        std::shared_ptr<AppModel> appModel = AppModel::createModelFromFile(appDevice, "./models/House.obj");
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                auto house = AppObject::createAppObject();
                house.model = appModel;
                house.texture = this->_loadedTextures["house"];
                house.transform.translation = { -n*10/2 + 10 * i/1.5 + 5, 5, -n * 10/2 + 10 * j/1.5 + 5};
                //cube.transform.rotation = { 3.14 / 2, 0.0, 0.0f };
                house.transform.rotation = { 3.14, 0.0, 0.0f };
                house.transform.scale = { 0.5f, 0.5f, 0.5f };
                //cube.transform.scale = { 2.0f, 2.0f, 2.0f };
                appObjects.emplace(house.getId(), std::move(house));
            }
        }

       
    }
    void Application::loadTextures()
    {
        this->_loadedTextures["tommy"] = AppTexture::createTextureFromFile(appDevice, "./textures/tommy.png");
        this->_loadedTextures["house"] = AppTexture::createTextureFromFile(appDevice, "./textures/houseTextures/House_Albedo.png");        
    }
    
};


