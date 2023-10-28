#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "../appPipeline.hpp"
#include "../appDevice.hpp"
#include "../objects/appObject.hpp"
#include "../appCamera.hpp"
#include "../frameInfo.hpp"
#include <memory>
#include <vector>

namespace appNamespace {
	class ImGuiRenderSystem {
	public:
		ImGuiRenderSystem(AppWindow &window, AppDevice &device, VkRenderPass renderPass,
			VkDescriptorPool descriptorPool);
		~ImGuiRenderSystem();

		ImGuiRenderSystem(const ImGuiRenderSystem&) = delete;
		ImGuiRenderSystem& operator=(const ImGuiRenderSystem&) = delete;
		void showDemo(bool show_demo_window=true);
		void render(FrameInfo& frameInfo);
	private:
		ImGuiIO io{};
		void appObjectsList(AppObject::Map& appObjects);
		/*AppDevice &appDevice;
		std::unique_ptr<AppPipeline> appPipeline;
		VkPipelineLayout pipelineLayout;*/

	};
};