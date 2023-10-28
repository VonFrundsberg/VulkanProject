add_library(PhysicsLibrary
    intersectionSystem.cpp
    physicsSystem.cpp
)
target_include_directories(PhysicsLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${Vulkan_INCLUDE_DIR}/glm"
