#pragma once

#include "../appPipeline.hpp"
#include "../appDevice.hpp"
#include "../appObject.hpp"
#include "../appCamera.hpp"
#include "../frameInfo.hpp"
#include <memory>
#include <vector>

namespace appNamespace {
	class SimpleRenderSystem {
	public:
		SimpleRenderSystem(AppDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		void renderAppObjects(FrameInfo& frameInfo);
	private:
		AppDevice &appDevice;
		std::unique_ptr<AppPipeline> appPipeline;
		VkPipelineLayout pipelineLayout;

		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
	};
};