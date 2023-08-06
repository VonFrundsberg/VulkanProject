#pragma once

#include "appModel.hpp"

#include <memory>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/euler_angles.hpp>
namespace appNamespace {
	struct TransformComponent{
		glm::vec4 translation{};
		glm::vec4 scale{1.0f, 1.0f, 1.0f, 1.0f};
		glm::vec4 rotation{ 0.0f, 0.0f, 0.0f, 0.0f };
		glm::mat4 mat4() { 
			glm::mat4 scaleMat = glm::diagonal4x4(scale);
			glm::mat4 rotatMat = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
			return rotatMat * scaleMat; }
	};
	class AppObject {
	public:
		using id_t = unsigned int;
		static AppObject createAppObject(){
			static id_t currentId = 0;
			return AppObject(currentId++);
		}

		AppObject(const AppObject&) = delete;
		AppObject& operator=(const AppObject&) = delete;
		AppObject(AppObject&&) = default;
		AppObject& operator=(AppObject&&) = default;

		id_t getId(){ return id; }

		std::shared_ptr<AppModel> model{};
		glm::vec4 color{};
		TransformComponent transform{};
	private:
		AppObject(id_t objectId) : id{ objectId } {};
		id_t id;

	};
}