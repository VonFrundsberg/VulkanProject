#pragma once
#include <string>
#include <vector>

#include "appDevice.hpp"

namespace appNamespace {
	struct PipelineConfigInfo {
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;


		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};
	class AppPipeline {
	public:
		AppPipeline(const std::string& vertFilePath, const std::string& fragFilePath,
			AppDevice& device, const PipelineConfigInfo& configInfo);
		~AppPipeline();

		AppPipeline(const AppPipeline&) = delete;
		AppPipeline& operator=(const AppPipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);

		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo &configInfo);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
		AppDevice& appDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule VertShaderModule;
		VkShaderModule FragShaderModule;
		VkPipelineCache pipelineCache;
	};
};