#pragma once

#include "appWindow.hpp"
#include "appPipeline.hpp"
#include "appDevice.hpp"
#include "appSwapChain.hpp"

#include <memory>
#include <vector>

namespace appNamespace {
	class Application {
	public:
		const uint32_t WIDTH = 800;
		const uint32_t HEIGHT = 600;
		void run();

		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&)=delete;

	private:
		appWindow window{ WIDTH, HEIGHT, "Application name" };
		appDevice appDevice{ window };
		appSwapChain appSwapChain{ appDevice, window.getExtent() };
		std::unique_ptr<AppPipeline> appPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;

		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

	};
};