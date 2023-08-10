#pragma once

#include "appPipeline.hpp"
#include "appDevice.hpp"
#include "appObject.hpp"
#include "appCamera.hpp"
#include <memory>
#include <vector>

namespace appNamespace {
	class SimpleRenderSystem {
	public:
		SimpleRenderSystem(AppDevice &device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		void renderAppObjects(VkCommandBuffer commandBuffer, std::vector<AppObject>& appObjects, const AppCamera& camera);
	private:
		AppDevice &appDevice;
		std::unique_ptr<AppPipeline> appPipeline;
		VkPipelineLayout pipelineLayout;

		void createPipelineLayout();
		void createPipeline(const std::string& vertFilePath, const std::string& fragFilePath, VkRenderPass renderPass);
	};
};