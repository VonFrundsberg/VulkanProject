add_subdirectory(ImGui)
add_library(RenderLibrary STATIC
    ImGui_RenderSystem.cpp
    simpleRenderSystem.cpp)

target_include_directories(RenderLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${GLFW_SOURCE_DIR}/include"
    "${Vulkan_INCLUDE_DIR}"
    "${Vulkan_INCLUDE_DIR}/glm"
                           )
target_link_libraries(RenderLibrary PUBLIC
    glfw
    Vulkan::Vulkan
    ImGuiLibrary
    )