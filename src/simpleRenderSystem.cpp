#include "SimpleRenderSystem.hpp"
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
	SimpleRenderSystem::SimpleRenderSystem(AppDevice& device, VkRenderPass renderPass): appDevice{device}
	{ 
		createPipelineLayout();
		createPipeline("shaders/vert.spv", "shaders/frag.spv", renderPass);
	}
	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(appDevice.device(), pipelineLayout, nullptr);
	}
 
	void SimpleRenderSystem::createPipelineLayout()
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
	void SimpleRenderSystem::createPipeline(const std::string& vertFilePath, const std::string& fragFilePath, VkRenderPass renderPass) {
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");
		PipelineConfigInfo pipelineConfig{};
		AppPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		appPipeline = std::make_unique<AppPipeline>(vertFilePath, fragFilePath, appDevice, pipelineConfig);
	}
	void SimpleRenderSystem::renderAppObjects(VkCommandBuffer commandBuffer, std::vector<AppObject>& appObjects, const AppCamera& camera)
	{
		appPipeline->bind(commandBuffer);
		auto projectionView = camera.getProjection() * camera.getView();
		for (auto& object : appObjects) {
			SimplePushConstantData push{};	
			auto modelMatrix = object.transform.mat4();
			push.transform = projectionView * modelMatrix;
			push.normalMatrix = object.transform.normalMatrix();
			vkCmdPushConstants(commandBuffer, pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData),
				&push);
			object.model->bind(commandBuffer);
			object.model->draw(commandBuffer);
		}
	}
};


