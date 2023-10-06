include(FetchContent)
FetchContent_Declare(
  GLFW
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        7482de6071d21db77a7236155da44c172a7f6c9e # version 3.3.8
)
FetchContent_MakeAvailable(GLFW)

add_library(ControllerLibrary
    keyboardController.cpp
    mouseController.cpp
)
target_include_directories(ControllerLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${GLFW_SOURCE_DIR}/include"
    "${Vulkan_INCLUDE_DIR}"
    "${Vulkan_INCLUDE_DIR}/glm"
                           )
target_link_libraries(ControllerLibrary PUBLIC
    glfw
    Vulkan::Vulkan
    )