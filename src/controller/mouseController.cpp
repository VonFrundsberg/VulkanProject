#include "mouseController.hpp"

namespace appNamespace {
	MouseController::MouseController(GLFWwindow* window, bool disableCursor)
	{
		if (disableCursor) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	void MouseController::moveInPlaneXZ(GLFWwindow* window, float dt, AppObject& object)
	{
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (state == GLFW_TRUE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glm::vec3 rotate{ yPos, -xPos, 0.0f };
			glfwGetCursorPos(window, &xPos, &yPos);
			rotate.x -= yPos;
			rotate.y += xPos;
			if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
				object.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
			}

			//object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -1.5f, 1.5f);
			object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -1.5f, 1.5f);
			object.transform.rotation.y = glm::mod(object.transform.rotation.y, glm::two_pi<float>());
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}
