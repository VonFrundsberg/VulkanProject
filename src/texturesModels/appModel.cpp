#include "appModel.hpp"
#include "../appUtils.hpp"
#include "appModel.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "glTF_Loader.hpp"
#include <cassert>
#include <iostream>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <gtx/string_cast.hpp>

namespace std {
	template<>
	struct hash<appNamespace::Vertex> {
		size_t operator()(appNamespace::Vertex const& vertex) const {
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
		createJointMatrices(builder.invMatrices);
	}
	AppModel::~AppModel() {	
	}
	void AppModel::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer->getBuffer() };
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer) {
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32); 
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
		uint32_t vertexSize = sizeof(vertices[0]);

		Buffer stagingBuffer{ appDevice, vertexSize, vertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };

		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)vertices.data());

		vertexBuffer = std::make_unique<Buffer>(appDevice, vertexSize, vertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		appDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
	}
	void AppModel::createIndexBuffers(const std::vector<uint32_t>& indices) {
		indexCount = static_cast<uint32_t>(indices.size());
		this->hasIndexBuffer = indexCount > 0;
		if (!hasIndexBuffer) return;

		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
		uint32_t indexSize = sizeof(indices[0]);

		Buffer stagingBuffer{ appDevice, indexSize, indexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };

		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)indices.data());

		indexBuffer = std::make_unique<Buffer>(appDevice, indexSize, indexCount,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		appDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
	}

	void AppModel::createJointMatrices(const std::vector<glm::mat4> invMatricesArg)
	{
		this->invMatrices = invMatricesArg;
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

					/*vertex.color = { attrib.colors[3 * index.vertex_index + 0],
									attrib.colors[3 * index.vertex_index + 1],
									attrib.colors[3 * index.vertex_index + 2] };*/
				}
				if (index.normal_index >= 0) {
					vertex.normal = { attrib.normals[3 * index.normal_index + 0],
										attrib.normals[3 * index.normal_index + 1],
										attrib.normals[3 * index.normal_index + 2] };

				}
				if (index.texcoord_index >= 0) {
					vertex.uv = { attrib.texcoords[2 * index.texcoord_index + 0],
									1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};
					/*vertex.uv = { attrib.texcoords[2 * index.texcoord_index + 0],
									attrib.texcoords[2 * index.texcoord_index + 1] };*/

				}
				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}
	void AppModel::Builder::load_glTF_Model(const std::string& filepath)
	{

		vertices.clear();
		indices.clear();

		auto gltfObject = GLTF_Loader(filepath);
		if (!gltfObject.skins.empty()) {
			for (const auto& skin : gltfObject.skins) {
				 std::vector<glm::mat4> invMatricesTmp;
				 std::vector<glm::mat4> globalTransformMatrices;
				 gltfObject.getInverseSkinMatrices(invMatricesTmp, skin.first);
				 
				 std::vector<int> jointsVector;
				 gltfObject.getJointsVector(jointsVector, skin.first);

				 this->invMatrices.resize(jointsVector.size());
				 globalTransformMatrices.resize(jointsVector.size());
				 for (const int& nodeNumber : jointsVector) {
					 const auto & translation = gltfObject.nodes[nodeNumber].translation;
					 std::vector<float> scale = gltfObject.nodes[nodeNumber].scale;
					 if (scale[0]*scale[0] + scale[1]*scale[1] + scale[2]*scale[2] <= 0.01) {
						 scale[0] = 1.0;  scale[1] = 1.0; scale[2] = 1.0;
					 }
					 const auto& rotation = gltfObject.nodes[nodeNumber].rotation;
					 const auto& identityMatrix = glm::mat4{ 1.0f };
					 //std::cout << glm::to_string(identityMatrix);
					// std::cout << "number " << nodeNumber << '\n';
					 glm::mat4 translateMatrix = glm::translate(identityMatrix,
						 glm::vec3{ translation[0], translation[1], translation[2]});
					 //std::cout << glm::to_string(translateMatrix) << "\n";
					 glm::mat4 scaleMatrix = glm::scale(identityMatrix, 
						 glm::vec3{ scale[0], scale[1], scale[2]});
					 //std::cout << glm::to_string(scaleMatrix) << "\n";

					 glm::quat rotationQuat{rotation[3], rotation[0], rotation[1], rotation[2]};
					 glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);
					 glm::mat4 transformMatrix = translateMatrix * rotationMatrix * scaleMatrix;
					 globalTransformMatrices[nodeNumber] = transformMatrix;
				 }
				 for (const int& nodeNumber : jointsVector) {
					 const auto& children = gltfObject.nodes[nodeNumber].children;
					 for (const int& childNumber : children) {
						 globalTransformMatrices[childNumber] =
							 globalTransformMatrices[nodeNumber] * globalTransformMatrices[childNumber];
					 }
				 }
				 int i = 0;
				 for (const int& nodeNumber : jointsVector) {
					 this->invMatrices[i] = globalTransformMatrices[nodeNumber] * invMatricesTmp[i];
					 i++;
				 }
			}
		}
		for (const auto& mesh : gltfObject.meshes) {
			std::vector<float> modelPositions;
			gltfObject.getMeshData(modelPositions, mesh.first, "POSITION");

			std::vector<float> modelNormals;
			gltfObject.getMeshData(modelNormals, mesh.first, "NORMAL");

			std::vector<float> modelUV;
			gltfObject.getMeshData(modelUV, mesh.first, "TEXCOORD_0");
			std::vector<unsigned char> modelJoints;
			std::vector<float> modelWeights;
			if (!gltfObject.skins.empty()) {
				gltfObject.getMeshData(modelJoints, mesh.first, "JOINTS_0");				
				gltfObject.getMeshData(modelWeights, mesh.first, "WEIGHTS_0");
				//gltfObject.printMeshData(mesh.first, "JOINTS_0");
			}

			std::vector<unsigned short> modelIndices;
			gltfObject.getMeshData(modelIndices, mesh.first, "indices");
			indices.reserve(modelIndices.size());

			for (const auto& obj : modelIndices) {
				indices.push_back(static_cast<uint32_t>(obj));
			}
			const auto verticesAmount = static_cast<int>(modelPositions.size() / 3);
			for (int i = 0; i < verticesAmount; i++) {
				Vertex vertex{};
				vertex.position = { modelPositions[3 * i + 0],
									modelPositions[3 * i + 1],
									modelPositions[3 * i + 2] };

				vertex.normal = {
					modelNormals[3 * i + 0],
					modelNormals[3 * i + 1],
					modelNormals[3 * i + 2] };

				vertex.uv = { modelUV[2 * i + 0], modelUV[2 * i + 1] };

				vertex.color = { 1.0f, 1.0f, 1.0f };

				if (!modelJoints.empty()) {
					vertex.joints = { static_cast<float>(modelJoints[4 * i + 0]),
						static_cast<float>(modelJoints[4 * i + 1]),
						static_cast<float>(modelJoints[4 * i + 2]),
						static_cast<float>(modelJoints[4 * i + 3]) };
					vertex.weights = { modelWeights[4 * i + 0],
						modelWeights[4 * i + 1],
						modelWeights[4 * i + 2],
						modelWeights[4 * i + 3]};
					//vertex.weights = { 1.0, 0.0, 0.0, 0.0};
				}
				vertices.push_back(vertex);
			}

			
		}
	}
	std::unique_ptr<AppModel> AppModel::createModelFromFile(AppDevice& device, const std::string& filepath)
	{
		AppModel::Builder builder{};
		//builder.loadModel(filepath);
		builder.load_glTF_Model(filepath);
		std::cout << "unique vertices count of " << filepath << " is " << builder.vertices.size() << std::endl;
		return std::make_unique<AppModel>(device, builder);
	}
}
