add_subdirectory(controller)
add_subdirectory(renderSystems)
add_subdirectory(texturesModels)
add_subdirectory(objects)

add_library(SrcLibrary STATIC
    appWindow.cpp
    appDevice.cpp
    appRenderer.cpp
    buffers.cpp
    descriptors.cpp
    appCamera.cpp
    appPipeline.cpp
    appSwapChain.cpp
)
target_include_directories(SrcLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${GLFW_SOURCE_DIR}/include"
    "${Vulkan_INCLUDE_DIR}"
    "${Vulkan_INCLUDE_DIR}/glm"
                           )
target_link_libraries(SrcLibrary PUBLIC
    glfw
    Vulkan::Vulkan
    ControllerLibrary
    RenderLibrary
    TexModLibrary
    ObjectLibrary
    )

   # message(CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
  # message(GLFW_SOURCE_DIR="${GLFW_SOURCE_DIR}/include")
   