#pragma once
#include "src/appWindow.hpp"
#include "src/appDevice.hpp"
#include "src/appRenderer.hpp"
#include "src/appObject.hpp"
#include "src/texturesModels/appTexture.hpp"
#include "src/descriptors.hpp"
#include <memory>
#include <vector>

namespace appNamespace {
	class Application {
	public:
		const int WIDTH = 800;
		const int HEIGHT = 600;
		const float MAX_FRAME_TIME = 0.1f;
		const int FPS_CAP = 100;
		void run();

		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&)=delete;

	private:
		void loadObjects();
		void loadTextures();
		VkAllocationCallbacks* g_Allocator{ nullptr };

		AppWindow appWindow{ WIDTH, HEIGHT, "Application name" };
		AppDevice appDevice{ appWindow };
		AppRenderer appRenderer{ appWindow, appDevice };
		std::unordered_map<std::string, std::unique_ptr<DescriptorPool>> globalPools;
		AppObject::Map appObjects;
		std::unordered_map<std::string, std::shared_ptr<AppTexture>> _loadedTextures;
	};
};