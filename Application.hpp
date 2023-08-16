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
		AppWindow appWindow{ WIDTH, HEIGHT, "Application name" };
		AppDevice appDevice{ appWindow };
		AppRenderer appRenderer{ appWindow, appDevice };


		std::unique_ptr<DescriptorPool> globalPool{};
		std::unique_ptr<DescriptorPool> globalPool1{};
		AppObject::Map appObjects;  
		//std::shared_ptr<AppTexture> texture;
		std::unordered_map<std::string, std::shared_ptr<AppTexture>> _loadedTextures;
	};
};