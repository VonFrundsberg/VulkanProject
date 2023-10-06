cmake_minimum_required(VERSION 3.10)

project(VulkanProject VERSION 1.0)

configure_file(VulkanProjectConfig.h.in VulkanProjectConfig.h)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  GLFW
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        7482de6071d21db77a7236155da44c172a7f6c9e # version 3.3.8
)
FetchContent_MakeAvailable(GLFW)

file(GLOB SOURCES "*.cpp")
file(GLOB SUBFOLDER_SOURCES "src/*.cpp")

add_executable(VulkanProject
    ${SOURCES}
    ${SUBFOLDER_SOURCES})

#set(ENV{VULKAN_SDK} "c:/VulkanSDK/1.3.261.1/")
find_package(Vulkan REQUIRED)
target_include_directories(VulkanProject PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    "${GLFW_SOURCE_DIR}/include"
    "${Vulkan_INCLUDE_DIR}"
    "${Vulkan_INCLUDE_DIR}/glm"
                           )

target_link_libraries(VulkanProject 
    ${GLFW}
    ${Vulkan})

