#pragma once
#include <string>
#include <vector>

#include "appDevice.hpp"

namespace appNamespace {
	struct pipelineConfigInfo {
		VkViewport viewport;
		VkRect2D scissor; 
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};
	class AppPipeline {
	public:
		AppPipeline(const std::string& vertFilePath, const std::string& fragFilePath,
			AppDevice& device, const pipelineConfigInfo& configInfo);
		~AppPipeline();

		AppPipeline(const AppPipeline&) = delete;
		void operator=(const AppPipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);

		static pipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const pipelineConfigInfo &configInfo);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
		AppDevice& appDevice;
		VkPipeline GraphicsPipeline;
		VkShaderModule VertShaderModule;
		VkShaderModule FragShaderModule;
	};
};