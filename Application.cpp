#include "Application.hpp"
#include "src/appCamera.hpp"
#include "src/renderSystems/simpleRenderSystem.hpp"
#include "src/renderSystems/ImGui_RenderSystem.hpp"
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
#include <gtc/matrix_transform.hpp>

namespace appNamespace {
    struct GlobalUBO {
        alignas(16) glm::mat4 projectionView{ 1.0f };
        alignas(16) glm::vec3 directionToLight = glm::normalize(glm::vec3{ 1.0f, 1.0f, 1.0f });
        alignas(16) glm::mat4 jointMatrices[100];
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


        this->initDescriptorSets(unorderedMapOfglobalDescriptorSets, globalSetLayout, uboBuffers);

		SimpleRenderSystem simpleRenderSystem{ appDevice, appRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout() };
        ImGuiRenderSystem imGuiRendering{ appWindow, appDevice,
            appRenderer.getSwapChainRenderPass(),
            globalPools.at("ImGui")->getDescriptorPool()};

        AppCamera camera{};
        glm::vec3 cameraDistanceToPlayer = {0.0f, -1.0f, -1.0f };
        //glm::vec3 cameraRotation = { -3.14f, -3.14f, 0.0f };
        glm::vec3 initCameraTranslation = { 0.0f, -0.5f, 0.0f };
        auto viewerObject = AppObject::createAppObject();
        viewerObject.transform.translation = initCameraTranslation;

        KeyboardController keyboardCameraController{};
        MouseController mouseCameraController{appWindow.getGLFWwindow(), false};

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
            //appObjects[0].transform.rotation.z = viewerObject.transform.rotation.z;
            //appObjects[0].transform.rotation.x = viewerObject.transform.rotation.x - 3.14/2.0;
            appObjects[0].transform.rotation.y = viewerObject.transform.rotation.y;
            //appObjects[0].transform.normalMatrix
            float aspect = appRenderer.getAspectRatio();
            //appObjects[rand() % appObjects.size()].isTarget = 2;
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 1000.0f);

			if (auto commandBuffer = appRenderer.beginFrame()) {
                int frameIndex = appRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    dt,
                    commandBuffer,
                    camera, 
                    appObjects};

                //update 
                GlobalUBO ubo{};
                glm::mat4 identityMatrix = glm::mat4(1.0f);
                for (const auto& obj : appObjects) {
                    if (!obj.second.model.get()->invMatrices.empty()) {
                       /* ubo.jointMatrices[0] = obj.second.model.get()->invMatrices[2];
                        ubo.jointMatrices[1] = obj.second.model.get()->invMatrices[0];
                        ubo.jointMatrices[2] = obj.second.model.get()->invMatrices[1];*/

                        /*ubo.jointMatrices[0] = obj.second.model.get()->invMatrices[5];
                        ubo.jointMatrices[1] = obj.second.model.get()->invMatrices[0];
                        ubo.jointMatrices[2] = obj.second.model.get()->invMatrices[2];

                        ubo.jointMatrices[3] = obj.second.model.get()->invMatrices[1];
                        ubo.jointMatrices[4] = obj.second.model.get()->invMatrices[4];
                        ubo.jointMatrices[5] = obj.second.model.get()->invMatrices[3];

                        ubo.jointMatrices[6] = obj.second.model.get()->invMatrices[9];
                        ubo.jointMatrices[7] = obj.second.model.get()->invMatrices[8];
                        ubo.jointMatrices[8] = obj.second.model.get()->invMatrices[7];

                        ubo.jointMatrices[9] = obj.second.model.get()->invMatrices[6];
                        ubo.jointMatrices[10] = obj.second.model.get()->invMatrices[13];
                        ubo.jointMatrices[11] = obj.second.model.get()->invMatrices[12];

                        ubo.jointMatrices[12] = obj.second.model.get()->invMatrices[11];
                        ubo.jointMatrices[13] = obj.second.model.get()->invMatrices[10];*/
                        for (int i = 0; i < obj.second.model.get()->invMatrices.size(); i++) {
                            ubo.jointMatrices[i] = obj.second.model.get()->invMatrices[i];
                        }
                        /*for (int i = 0; i < ubo.jointMatrices->length(); i++) {
                            ubo.jointMatrices[i] = identityMatrix;
                        }*/
                    }
                }
                
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                //render
				appRenderer.beginSwapChainRenderPass(commandBuffer);

    			simpleRenderSystem.renderAppObjects(frameInfo);      
                imGuiRendering.render(frameInfo);

;				appRenderer.endSwapChainRenderPass(commandBuffer);
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
        loadTextures();
        loadObjects();

        for (auto &texture: this->_loadedTextures) {
            this->globalPools.emplace(texture.first, DescriptorPool::Builder(appDevice)
                .setMaxSets(AppSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, AppSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, AppSwapChain::MAX_FRAMES_IN_FLIGHT)
                .build());
        }
        this->globalPools.emplace("ImGui", DescriptorPool::Builder(appDevice)
            .setMaxSets(AppSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, AppSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build());
	}
	Application::~Application(){

	}  

    void Application::loadObjects() {
        
        int n = 1;
        std::shared_ptr<AppModel> appModelTommy = AppModel::createModelFromFile(appDevice, "./glTF/tommy.gltf");
        for (int j = 0; j < 1; j++) {
            for (int i = 0; i < 2; i++) {
                auto player = AppObject::createAppObject();
                player.model = appModelTommy;
                player.texture = this->_loadedTextures["tommy"];
                player.transform.translation = { 0, -0.5, 1.0 };
                //player.transform.rotation = { 0.0, 0.0f , 0.0};
                player.transform.rotation = { -3.14/2.0, 0.0f , 3.14f };
                //house.transform.scale = { 0.5f, 0.5f, 0.5f };
                player.transform.scale = { 0.5f, 0.5f, 0.5f };
                appObjects.emplace(player.getId(), std::move(player));
            }
        }
        //n = 1;
        //int d = 20;
        //std::shared_ptr<AppModel> appModelGrass = AppModel::createModelFromFile(appDevice, "./glTF/grass.gltf");
        //for (int j = 0; j < n; j++) {
        //    for (int i = 0; i < n; i++) {
        //        auto grass = AppObject::createAppObject();
        //        grass.model = appModelGrass;
        //        grass.texture = this->_loadedTextures["grass"];
        //        //-45
        //        /*house.transform.translation = { -n*d/2 + d * i + 15*d , -50, -n * d/2 + d * j + 15*d};*/
        //        grass.transform.translation = { 0, 0, 0};
        //        grass.transform.rotation = { 3.14, 0.0f , 0.0 };
        //        //house.transform.rotation = { 0.0, 0.0, 0.0f };
        //        //house.transform.scale = { 0.5f, 0.5f, 0.5f };
        //        grass.transform.scale = { 2.0f, 2.0f, 2.0f };
        //        appObjects.emplace(grass.getId(), std::move(grass));
        //    }
        //}
        std::shared_ptr<AppModel> appModelCube =
            AppModel::createModelFromFile(appDevice, "./glTF/complex/skeletal_tommy.gltf");
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                auto cube = AppObject::createAppObject();
                cube.model = appModelCube;
                cube.texture = this->_loadedTextures["tommy"];
                /*cube.transform.translation = { -n * 10 / 2 + 10 * i / 1.5 + 5, 5, -n * 10 / 2 + 10 * j / 1.5 + 5 };*/
                cube.transform.translation = { 0, -0.5, 1.2 };
                cube.transform.rotation = { 3.14, 3.14, 0.0f };
                //cube.transform.rotation = { -3.14 / 2, 0.0f , 3.14f /2 };
                //house.transform.scale = { 0.5f, 0.5f, 0.5f };
                cube.transform.scale = { 0.5f, 0.3f, 0.5f };
                appObjects.emplace(cube.getId(), std::move(cube));
            }
        }   
    }
    void Application::loadTextures()
    {
        this->_loadedTextures["tommy"] = 
            AppTexture::createTextureFromFile(appDevice, "./glTF/tommy.png", "tommy");
        this->_loadedTextures["grass"] =
            AppTexture::createTextureFromFile(appDevice, "./glTF/image.jpg", "grass");
    }

    void Application::initDescriptorSets(
        std::unordered_map<std::string, std::vector<VkDescriptorSet>> &unorderedMapOfglobalDescriptorSets,
        const std::unique_ptr<DescriptorSetLayout>& globalSetLayout,
        const std::vector<std::unique_ptr<Buffer>>& uboBuffers){

        for (auto& texture : this->_loadedTextures) {
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
    }
    
};


