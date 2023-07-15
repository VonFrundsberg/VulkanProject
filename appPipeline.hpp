#pragma once
#include <string>
#include <vector>

#include "appDevice.hpp"

namespace appNamespace {
	struct pipelineConfigInfo {
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};
	class appPipeline {
	public:
		appPipeline(const std::string& vertFilePath, const std::string& fragFilePath,
			appDevice& device);
		~appPipeline();

		appPipeline(const appPipeline&) = delete;
		void operator=(const appPipeline&) = delete;

		

	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
		static void defaultPipelineConfigInfo(pipelineConfigInfo& configInfo, uint32_t width, uint32_t height);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
		appDevice& AppDevice;
		VkPipeline GraphicsPipeline;
		VkShaderModule VertShaderModule;
		VkShaderModule FragShaderModule;
	};
};