#pragma once

#include "appWindow.hpp"
#include "appPipeline.hpp"
#include "appDevice.hpp"
namespace appNamespace {
	class Application {
	public:
		const uint32_t WIDTH = 800;
		const uint32_t HEIGHT = 600;
		void run();

	private:
		appWindow Window{ WIDTH, HEIGHT, "Application name" };
		appDevice AppDevice{ Window };
		pipelineConfigInfo configInfo{};
		appPipeline appPipeline{ "shaders/vert.spv", "shaders/frag.spv", AppDevice,
			appPipeline::defaultPipelineConfigInfo(HEIGHT, WIDTH)};
	};
};