#pragma once

#include <vector>
#include <map>
#include <memory>
#include "intersectionSystem.hpp"
#include "../objects/appObject.hpp"

namespace appNamespace {
	class PhysicsSystem {
	public:
		IntersectionSystem intersectionEngine;
		PhysicsSystem();
		void updateState(AppObject::Map & objectsVector, const int dt);
		static glm::vec4 getPlaneCoefficients(const glm::mat3 & matrix);
	private:
		glm::vec4 getPlaneCoefficientsGauss(const glm::mat3& matrix);
	};
}