#include "ImGui_RenderSystem.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace appNamespace {

	ImGuiRenderSystem::ImGuiRenderSystem(AppWindow& window, AppDevice& device, VkRenderPass renderPass, VkDescriptorPool descriptorPool)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		io.Fonts->AddFontFromFileTTF("Fonts/Cousine-Regular.ttf", 18);

		ImGui_ImplGlfw_InitForVulkan(window.getGLFWwindow(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = device.getInstance();
		init_info.PhysicalDevice = device.getPhysicalDevice();
		init_info.Device = device.device();
		//init_info.QueueFamily = appDevice.findPhysicalQueueFamilies().presentFamily;
		init_info.Queue = device.graphicsQueue();
		//init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = descriptorPool;
		init_info.Subpass = 0;
		init_info.MinImageCount = 2;
		init_info.ImageCount = 2;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		//init_info.Allocator = this->g_Allocator;
		//init_info.CheckVkResultFn = nullptr;
		ImGui_ImplVulkan_Init(&init_info, renderPass);

		{
			VkCommandBuffer commandBuffer = device.beginSingleTimeCommands();

			ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

			device.endSingleTimeCommands(commandBuffer);

			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}
	}
	ImGuiRenderSystem::~ImGuiRenderSystem()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
 
	
	void ImGuiRenderSystem::showDemo(bool show_demo_window)
	{
		//ImGui_ImplVulkan_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		ImGui::ShowDemoWindow(&show_demo_window);
		//ImGui::Render();
		//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer, 0);
	}
	void ImGuiRenderSystem::render(FrameInfo &frameInfo)
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//showDemo(true);
		appObjectsList(frameInfo.appObjects);

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), frameInfo.commandBuffer, 0);
	}
	void ImGuiRenderSystem::appObjectsList(AppObject::Map& appObjects)
	{
		bool p_open = true;
		static bool no_titlebar = false;
		static bool no_scrollbar = false;
		static bool no_menu = false;
		static bool no_move = false;
		static bool no_resize = false;
		static bool no_collapse = false;
		static bool no_close = false;
		static bool no_nav = false;
		static bool no_background = false;
		static bool no_bring_to_front = false;
		static bool no_docking = false;
		static bool unsaved_document = false;

		ImGuiWindowFlags window_flags = 0;
		if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
		if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
		if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
		if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
		if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
		if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
		if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
		if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (no_docking)         window_flags |= ImGuiWindowFlags_NoDocking;
		if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
		//if (no_close)           p_open = NULL; // Don't pass our bool* to Begin
		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Once);
		//std::cout << main_viewport->Size.x << "\n";
		ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);

		ImGui::Begin("Application objects", &p_open, window_flags);

		static int item_current_idx = 0;

		if (ImGui::BeginListBox("App objects list"))
		{
			for (auto & object: appObjects)
			{
				const bool is_selected = (item_current_idx == object.first);
				if (ImGui::Selectable(std::to_string(object.first).c_str(), is_selected)) {
					auto& objectPrev = appObjects.at(item_current_idx);
					objectPrev.isTarget = 0;

					item_current_idx = object.first;

					auto& object = appObjects.at(item_current_idx);
					object.isTarget = 1;
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		
		ImGui::InputFloat3("position", glm::value_ptr(appObjects.at(item_current_idx).transform.translation));
		ImGui::InputFloat3("rotation", glm::value_ptr(appObjects.at(item_current_idx).transform.rotation));
		ImGui::InputFloat3("scale", glm::value_ptr(appObjects.at(item_current_idx).transform.scale));
		//ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
		char* str = appObjects.at(item_current_idx).texture->getName().data();
		ImGui::InputText("texture", str, IM_ARRAYSIZE(str));

		static int clicked = 0;
		if (ImGui::Button("Add copy of target"))
			clicked++;
		if (clicked & 1)
		{
			auto object = AppObject::createAppObject();
			//-45
			object.transform.translation = { 0 ,0, 0};
			object.model = appObjects.at(item_current_idx).model;
			object.texture = appObjects.at(item_current_idx).texture;
			object.transform.rotation = { 0.0f, 0.0f, 0.0f };
			object.transform.scale = { 1.0f, 1.0f, 1.0f };
			appObjects.emplace(object.getId(), std::move(object));
			clicked = 0;
		}
		ImGui::End();

	}
};


