#pragma once
#include <string>
#include <vector>

#include "appDevice.hpp"


struct pipelineConfigInfo {};

class appPipeline {
public:
	appPipeline(const std::string& vertFilePath, const std::string& fragFilePath,
		appDevice &device, const pipelineConfigInfo &configInfo);
	~appPipeline() {};

	appPipeline(const appPipeline&) = delete;
	void operator=(const appPipeline&) = delete; 

	static pipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

private:
	static std::vector<char> readFile(const std::string& filePath);
	void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const pipelineConfigInfo& configInfo);
	void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	appDevice& AppDevice;
	VkPipeline GraphicsPipeline;
	VkShaderModule VertShaderModule;
	VkShaderModule FragShaderModule;
};