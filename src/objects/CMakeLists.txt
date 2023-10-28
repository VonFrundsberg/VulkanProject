add_library(ObjectLibrary
    appObject.cpp
    intersectionComponent.cpp
)
target_include_directories(ObjectLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${Vulkan_INCLUDE_DIR}/glm"
                           )
target_link_libraries(ObjectLibrary PUBLIC
    TexModLibrary
    )