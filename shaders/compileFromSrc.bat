set VULKAN_SDK_PATH=%VULKAN_SDK%\Bin

%VULKAN_SDK_PATH%\glslc.exe shaders\shader.vert -o shaders\vert.spv
%VULKAN_SDK_PATH%\glslc.exe shaders\shader.frag -o shaders\frag.spv
%VULKAN_SDK_PATH%\glslc.exe shaders\ui_shader.vert -o shaders\ui_vert.spv
%VULKAN_SDK_PATH%\glslc.exe shaders\ui_shader.frag -o shaders\ui_frag.spv