#pragma once
#include "../appDevice.hpp"
#include "../buffers.hpp"
//#include "appModel.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>
namespace appNamespace {
	class AppTexture {
	public:
		struct TextureInfo {
			void* pixels;
			int texWidth;
			int texHeight;
			int texChannels;
			void loadTexture(const std::string& filepath);
		};
		AppTexture(AppDevice& device, const AppTexture::TextureInfo& texInfo, const std::string& textureName = "");
		~AppTexture();

		AppTexture(const AppTexture&) = delete;
		AppTexture& operator=(const AppTexture&) = delete;

		static std::unique_ptr<AppTexture> createTextureFromFile(AppDevice& device, const std::string& filepath, const std::string& textureName = "");
		VkImageView getTextureImageView() { return textureImageView; }
		std::string& getName() { return name; }
		void setName(const std::string& argName) { name = argName; }
		std::vector<VkDescriptorSet>* textureDescriptors;
	private:
		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void createTextureImageView();
		AppDevice& appDevice;
		std::string name;
		VkImageView textureImageView{};
		VkImage textureImage{};
		VkDeviceMemory textureImageMemory{};
		

	};
}