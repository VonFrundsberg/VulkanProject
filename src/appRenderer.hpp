#pragma once
#include <cassert>
#include "appWindow.hpp"
#include "appDevice.hpp"
#include "appSwapChain.hpp"
#include <memory>
#include <vector>


namespace appNamespace {
	class AppRenderer {
	public:

		AppRenderer(AppWindow &window, AppDevice &device);
		~AppRenderer();

		AppRenderer(const AppRenderer&) = delete;
		AppRenderer& operator=(const AppRenderer&) = delete; 


		VkRenderPass getSwapChainRenderPass() const { return appSwapChain->getRenderPass(); }
		float getAspectRatio() const { return appSwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer getCurrentCommandBuffer() const { 
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		AppWindow& appWindow;
		AppDevice& appDevice;
		std::unique_ptr<AppSwapChain> appSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };
		bool isFrameStarted{ false };
	};
};