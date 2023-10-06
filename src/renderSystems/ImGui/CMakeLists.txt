#find_package(Vulkan REQUIRED)

add_library(ImGuiLibrary STATIC
    imgui_impl_glfw.cpp
    imgui_impl_vulkan.cpp
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_stdlib.cpp
    imgui_widgets.cpp
    imgui_tables.cpp
)
target_include_directories(ImGuiLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${GLFW_SOURCE_DIR}/include"
    "${Vulkan_INCLUDE_DIR}/vulkan")

target_link_libraries(ImGuiLibrary PUBLIC
    glfw
    Vulkan::Vulkan
    )
message(CURRENT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
message(PROJECT_SOURCE_DIR="${Vulkan_INCLUDE_DIR}")