#include "Application.hpp"
#include <iostream>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace appNamespace {

	struct SimplePushConstantData {
		 glm::vec4 offset;
		 glm::vec4 color;
		 glm::mat4 transform{ 1.0f };
};
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
		loadObjects();
		createPipelineLayout();
		recreateSwapChain("shaders/vert.spv", "shaders/frag.spv");
		createCommandBuffers();
	}
	Application::~Application()
	{
		vkDestroyPipelineLayout(appDevice.device(), pipelineLayout, nullptr);
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
	void Application::renderAppObjects(VkCommandBuffer commandBuffer)
	{
		appPipeline->bind(commandBuffer);
		for (auto& object : appObjects) {

			object.transform.rotation.z = glm::mod(object.transform.rotation.z + 0.01f, glm::two_pi<float>());
			SimplePushConstantData push{};
			push.offset = object.transform.translation;
			push.color = object.color;
			push.transform = object.transform.mat4();
			vkCmdPushConstants(commandBuffer, pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData),
				&push);
			object.model->bind(commandBuffer);
			object.model->draw(commandBuffer);
		}
	}
	void Application::createPipelineLayout()
	{

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);


		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

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
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
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

		this->renderAppObjects(commandBuffers[i]);		

		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
};


