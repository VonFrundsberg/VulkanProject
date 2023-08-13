#pragma once

#include "../appPipeline.hpp"
#include "../appDevice.hpp"
#include "../appObject.hpp"
#include "../appCamera.hpp"
#include "../frameInfo.hpp"
#include <memory>
#include <vector>

namespace appNamespace {
	class UIRenderSystem {
	public:
		UIRenderSystem(AppDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~UIRenderSystem();

		UIRenderSystem(const UIRenderSystem&) = delete;
		UIRenderSystem& operator=(const UIRenderSystem&) = delete;
		void render(FrameInfo& frameInfo);
	private:
		AppDevice &appDevice;
		std::unique_ptr<AppPipeline> appPipeline;
		VkPipelineLayout pipelineLayout;

		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
	};
};