cmake_minimum_required(VERSION 3.10)

project(VulkanProject VERSION 1.0)

configure_file(VulkanProjectConfig.h.in VulkanProjectConfig.h)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Vulkan REQUIRED)
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        7482de6071d21db77a7236155da44c172a7f6c9e # version 3.3.8
)
FetchContent_MakeAvailable(glfw)

add_executable(VulkanProject 
Source.cpp
Application.cpp)
add_subdirectory(src)


target_include_directories(VulkanProject  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${GLFW_SOURCE_DIR}/include"
    "${Vulkan_INCLUDE_DIR}"
    "${Vulkan_INCLUDE_DIR}/glm"
                           )
target_link_libraries(VulkanProject PUBLIC
    glfw
    Vulkan::Vulkan
    SrcLibrary
    )


