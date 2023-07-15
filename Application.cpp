#include "Application.hpp"
#include <iostream>

namespace appNamespace {
	void Application::run()
	{
		while (!window.shouldClose()) {
			glfwPollEvents();
		}
	}
	Application::Application()
	{
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}
	Application::~Application()
	{
		vkDestroyPipelineLayout(appDevice.device(), pipelineLayout, nullptr);
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
	void Application::createPipeline(){
		auto pipelineConfig = AppPipeline::defaultPipelineConfigInfo(appSwapChain.width(), appSwapChain.height());
		pipelineConfig.renderPass = appSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		appPipeline = std::make_unique<AppPipeline>("shaders/vert.spv", "shaders/frag.spv", appDevice, pipelineConfig);
	}
	void Application::createCommandBuffers()
	{
	}
	void Application::drawFrame()
	{
	}
};


