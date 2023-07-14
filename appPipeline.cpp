#include "appPipeline.hpp"
#include <fstream>
#include <iostream>

appPipeline::appPipeline(const std::string& vertFilePath, const std::string& fragFilePath,
	appDevice& device, const pipelineConfigInfo& configInfo) : AppDevice{ device }
{
	createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);

}

pipelineConfigInfo appPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
{
	pipelineConfigInfo configInfo{};
	configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	return configInfo;
}

std::vector<char> appPipeline::readFile(const std::string& filePath)
{
	std::ifstream file(filePath, (std::ios::ate) | (std::ios::binary));
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!" + filePath);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}
void appPipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const pipelineConfigInfo& configInfo)
{
	auto vertShaderCode = readFile(vertFilePath);
	auto fragShaderCode = readFile(fragFilePath);

	std::cout << vertShaderCode.size() << std::endl;
	std::cout << fragShaderCode.size() << std::endl;
}
void appPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();

	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	createInfo.flags = 0;

	if (vkCreateShaderModule(AppDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
}
