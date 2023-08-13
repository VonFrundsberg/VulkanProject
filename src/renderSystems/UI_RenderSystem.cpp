#include "UI_RenderSystem.hpp"
#include <iostream>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace appNamespace {
	struct SimplePushConstantData {
		glm::mat4 transform{ 1.0f };
		glm::mat4 normalMatrix{ 1.0f };
	};
	UIRenderSystem::UIRenderSystem(AppDevice& device, VkRenderPass renderPass): appDevice{device}
	{ 
		createPipelineLayout();
		createPipeline(renderPass);
	}
	UIRenderSystem::~UIRenderSystem()
	{
		vkDestroyPipelineLayout(appDevice.device(), pipelineLayout, nullptr);
	}
 
	void UIRenderSystem::createPipelineLayout()
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
	void UIRenderSystem::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");
		PipelineConfigInfo pipelineConfig{};
		const std::string vertFilePath = "shaders/vert.spv";
		const std::string fragFilePath = "shaders/frag.spv";
		AppPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		appPipeline = std::make_unique<AppPipeline>(vertFilePath, fragFilePath, appDevice, pipelineConfig);
	}
	void UIRenderSystem::renderAppObjects(FrameInfo&frameInfo, std::vector<AppObject>& appObjects)
	{
		appPipeline->bind(frameInfo.commandBuffer);
		auto projectionView = frameInfo.camera.getProjection() * frameInfo.camera.getView();
		for (auto& object : appObjects) {
			SimplePushConstantData push{};	
			auto modelMatrix = object.transform.mat4();
			push.transform = projectionView * modelMatrix;
			push.normalMatrix = object.transform.normalMatrix();
			vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData),
				&push);
			object.model->bind(frameInfo.commandBuffer);
			object.model->draw(frameInfo.commandBuffer);
		}
	}
};


