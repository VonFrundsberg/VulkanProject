add_library(TexModLibrary
	appModel.cpp
	appTexture.cpp
	modelUtils.cpp
	vertex.cpp
)
include(FetchContent)
FetchContent_Declare(
  glTF_VulkanLoader
  GIT_REPOSITORY https://github.com/VonFrundsberg/glTF_VulkanLoader.git
  GIT_TAG        00e2edd4245b34d98bec8517940fb98e521e1f12
)
FetchContent_MakeAvailable(glTF_VulkanLoader)

FetchContent_Declare(
  rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
  GIT_TAG        17254e090e0dc3d5d1aca8efd6303cdbd07dbae1 # version 1.1.0
)
FetchContent_MakeAvailable(rapidjson)

target_include_directories(TexModLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${GLFW_SOURCE_DIR}/include"
    "${Vulkan_INCLUDE_DIR}"
    "${Vulkan_INCLUDE_DIR}/glm"
    "${rapidjson_SOURCE_DIR}/include"
    "${glTF_VulkanLoader_SOURCE_DIR}"
)

target_link_libraries(TexModLibrary PUBLIC
    glfw
    Vulkan::Vulkan
    glTF_VulkanLoaderLib
    )
#message(gltf_SOURCE_DIR="${rapidjson_SOURCE_DIR}")