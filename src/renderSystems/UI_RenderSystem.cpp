#include "UI_RenderSystem.hpp"
#include <iostream>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace appNamespace {
	UIRenderSystem::UIRenderSystem(AppDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout): appDevice{device}
	{ 
		createPipelineLayout(globalSetLayout);
		createPipeline(renderPass);
	}
	UIRenderSystem::~UIRenderSystem()
	{
		vkDestroyPipelineLayout(appDevice.device(), pipelineLayout, nullptr);
	}
 
	void UIRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{

		/*VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);*/

		std::vector< VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(appDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}
	void UIRenderSystem::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");
		PipelineConfigInfo pipelineConfig{};
		const std::string vertFilePath = "shaders/ui_vert.spv";
		const std::string fragFilePath = "shaders/ui_frag.spv";
		AppPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		appPipeline = std::make_unique<AppPipeline>(vertFilePath, fragFilePath, appDevice, pipelineConfig);
	}
	void UIRenderSystem::render(FrameInfo&frameInfo)
	{
		appPipeline->bind(frameInfo.commandBuffer);
		vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
			&frameInfo.globalDescriptorSet, 0, nullptr);
		vkCmdDraw(frameInfo.commandBuffer, 3, 1, 0, 0);
		/*for (auto& kv : frameInfo.uiObjects) {
			auto& object = kv.second;
			if (object.isVisible()) {
				vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, 0,
					nullptr);
				object.model->bind(frameInfo.commandBuffer);
				object.model->draw(frameInfo.commandBuffer);
			}
		}*/
	}
};


