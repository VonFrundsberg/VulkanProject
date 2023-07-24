#include "Application.hpp"
#include <iostream>
#include <array>

namespace appNamespace {
	void Application::run()
	{
		while (!appWindow.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}
		vkDeviceWaitIdle(appDevice.device());
	}
	Application::Application()
	{	
		loadModels();
		createPipelineLayout();
		recreateSwapChain("shaders/vert.spv", "shaders/frag.spv");
		createCommandBuffers();
	}
	Application::~Application()
	{
		vkDestroyPipelineLayout(appDevice.device(), pipelineLayout, nullptr);
	}
	void Application::loadModels(){
		std::vector<AppModel::Vertex> vertices{
			{{0.0f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}}
		};
		appModel = std::make_unique<AppModel>(appDevice, vertices);
	}
	void Application::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(appDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}
	void Application::createPipeline(const std::string& vertFilePath, const std::string& fragFilePath){
		assert(appSwapChain != nullptr && "Cannot create pipeline before swap chain!");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");
		PipelineConfigInfo pipelineConfig{};
		AppPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = appSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		appPipeline = std::make_unique<AppPipeline>(vertFilePath, fragFilePath, appDevice, pipelineConfig);
	}
	void Application::createCommandBuffers(){
		commandBuffers.resize(appSwapChain->imageCount());
		
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = appDevice.getCommandPool(); 
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(appDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void Application::freeCommandBuffers()
	{
		vkFreeCommandBuffers(appDevice.device(), appDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void Application::drawFrame(){
		uint32_t imageIndex;
		auto result = appSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain("shaders/vert.spv", "shaders/frag.spv");
			return;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
		recordCommandBuffer(imageIndex);
		result = appSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || appWindow.wasWindowResized()) {
			appWindow.resetWindowResizedFlag();
			recreateSwapChain("shaders/vert.spv", "shaders/frag.spv");
			return;
		}
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	}
	void Application::recreateSwapChain(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		auto extent = appWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = appWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(appDevice.device());
		if (appSwapChain == nullptr) {
			appSwapChain = std::make_unique<AppSwapChain>(appDevice, extent);
		}
		else {
			appSwapChain = std::make_unique<AppSwapChain>(appDevice, extent, std::move(appSwapChain));
			if (appSwapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}
		
		createPipeline(vertFilePath, fragFilePath);
	}
	void Application::recordCommandBuffer(int imageIndex)
	{
		int i = imageIndex;
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = appSwapChain->getRenderPass();
		renderPassInfo.framebuffer = appSwapChain->getFrameBuffer(i);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = appSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(appSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(appSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, appSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor);

		appPipeline->bind(commandBuffers[i]);
		appModel->bind(commandBuffers[i]);
		appModel->draw(commandBuffers[i]);

		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
};


