#pragma once

#include "appModel.hpp"

#include <memory>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace appNamespace {
	struct TransformComponent{
		glm::vec3 translation{};
		glm::vec3 scale{1.0f, 1.0f, 1.0f};
		glm::vec3 rotation{};
        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		glm::mat4 mat4();

		glm::mat3 normalMatrix();
	};
	class AppObject {
	public:
		using id_t = unsigned int;
		static AppObject createAppObject(){
			static id_t currentId = 0;
            return AppObject{ currentId++ };
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