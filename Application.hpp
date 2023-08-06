#pragma once

#include "appWindow.hpp"
#include "appPipeline.hpp"
#include "appDevice.hpp"
#include "appSwapChain.hpp"
#include "appObject.hpp"
#include <memory>
#include <vector>

namespace appNamespace {
	class Application {
	public:
		const int WIDTH = 800;
		const int HEIGHT = 600;
		void run();

		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&)=delete;

	private:
		AppWindow appWindow{ WIDTH, HEIGHT, "Application name" };
		AppDevice appDevice{ appWindow };
		std::unique_ptr<AppSwapChain> appSwapChain;
		std::unique_ptr<AppPipeline> appPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<AppObject> appObjects;  


		void loadObjects();
		void renderAppObjects(VkCommandBuffer commandBuffer);
		void createPipelineLayout();
		void createPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain(const std::string& vertFilePath, const std::string& fragFilePath);
		void recordCommandBuffer(int imageIndex);
	};
};