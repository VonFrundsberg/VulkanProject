#pragma once

#include "appWindow.hpp"
#include "appDevice.hpp"
#include "appRenderer.hpp"
#include "appObject.hpp"
#include <memory>
#include <vector>

namespace appNamespace {
	class Application {
	public:
		const int WIDTH = 800;
		const int HEIGHT = 600;
		const float MAX_FRAME_TIME = 0.1f;
		void run();

		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&)=delete;

	private:
		AppWindow appWindow{ WIDTH, HEIGHT, "Application name" };
		AppDevice appDevice{ appWindow };
		AppRenderer appRenderer{ appWindow, appDevice };

		std::vector<AppObject> appObjects;  
		void loadObjects();
	};
};