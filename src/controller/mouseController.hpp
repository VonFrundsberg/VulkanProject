#pragma once
#include "../objects/appObject.hpp"
#include "../appWindow.hpp"

namespace appNamespace {
    class MouseController {
    public:
        MouseController(GLFWwindow* window, bool disableCursor);
        void moveInPlaneXZ(GLFWwindow* window, float dt, AppObject& object);
        double xPos{0.0}, yPos{0.0};
        float lookSpeed{ 2.5f };
    };
}