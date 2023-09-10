#pragma once
#include "../appDevice.hpp"
#include "../buffers.hpp"
#include "vertex.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace appNamespace {
	class AppModel {
	public:
		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};
			void loadModel(const std::string& filepath);
			void load_glTF_Model(const std::string& filepath);
		};

		AppModel(AppDevice& device, const AppModel::Builder &builder);
		~AppModel();

		AppModel(const AppModel&) = delete;
		AppModel& operator=(const AppModel&) = delete;

		static std::unique_ptr<AppModel> createModelFromFile(AppDevice& device, const std::string& filepath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		AppDevice& appDevice;


		std::unique_ptr<Buffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		std::unique_ptr<Buffer> indexBuffer;
		uint32_t indexCount;
	};
}