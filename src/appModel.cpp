#include "appModel.hpp"
#include "appUtils.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <cassert>
#include <iostream>


namespace std {
	template<>
	struct hash<appNamespace::AppModel::Vertex> {
		size_t operator()(appNamespace::AppModel::Vertex const& vertex) const {
			size_t seed = 0;
			appNamespace::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}

namespace appNamespace {
	AppModel::AppModel(AppDevice& device, const AppModel::Builder& builder) : appDevice(device) {
		createVertexBuffers(builder.vertices);
		createIndexBuffers(builder.indices);
	}
	AppModel::~AppModel() {
		vkDestroyBuffer(appDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(appDevice.device(), vertexBufferMemory, nullptr);
		if (this->hasIndexBuffer) {
			vkDestroyBuffer(appDevice.device(), indexBuffer, nullptr);
			vkFreeMemory(appDevice.device(), indexBufferMemory, nullptr);
		}
	}
	void AppModel::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer) {
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32); 
		}
	}
	void AppModel::draw(VkCommandBuffer commandBuffer) {
		if (hasIndexBuffer) {
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		}
		else {
			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
		}

	}
	void AppModel::createVertexBuffers(const std::vector<Vertex>& vertices){
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		appDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(appDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(appDevice.device(), stagingBufferMemory);

		appDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBuffer, vertexBufferMemory);

		appDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
		vkDestroyBuffer(appDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(appDevice.device(), stagingBufferMemory, nullptr);
	}
	void AppModel::createIndexBuffers(const std::vector<uint32_t>& indices) {
		indexCount = static_cast<uint32_t>(indices.size());
		this->hasIndexBuffer = indexCount > 0;
		if (!hasIndexBuffer) return;

		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		appDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(appDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(appDevice.device(), stagingBufferMemory);

		appDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer, indexBufferMemory);

		appDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);
		vkDestroyBuffer(appDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(appDevice.device(), stagingBufferMemory, nullptr);
	}

	std::vector<VkVertexInputBindingDescription> AppModel::Vertex::getBindingDescriptions() {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> AppModel::Vertex::getAtrributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
		attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
		attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
		attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv) });
		return attributeDescriptions;
	} 

	void AppModel::Builder::loadModel(const std::string& filepath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
			std::runtime_error(warn + err);
		}
		vertices.clear();
		indices.clear();

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex{};

				if (index.vertex_index >= 0) {
					vertex.position = { attrib.vertices[3 * index.vertex_index + 0],
										 attrib.vertices[3 * index.vertex_index + 1],
										 attrib.vertices[3 * index.vertex_index + 2] };

					vertex.color = { attrib.colors[3 * index.vertex_index + 0],
									attrib.colors[3 * index.vertex_index + 1],
									attrib.colors[3 * index.vertex_index + 2] };
				}
				if (index.normal_index >= 0) {
					vertex.normal = { attrib.normals[3 * index.normal_index + 0],
										attrib.normals[3 * index.normal_index + 1],
										attrib.normals[3 * index.normal_index + 2] };

				}
				if (index.texcoord_index >= 0) {
					vertex.uv = { attrib.texcoords[2 * index.texcoord_index + 0],
									attrib.texcoords[2 * index.texcoord_index + 1] };

				}
				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}
	std::unique_ptr<AppModel> AppModel::createModelFromFile(AppDevice& device, const std::string& filepath)
	{
		AppModel::Builder builder{};
		builder.loadModel(filepath);
		std::cout << builder.vertices.size() << std::endl;
		return std::make_unique<AppModel>(device, builder);
	}
}
