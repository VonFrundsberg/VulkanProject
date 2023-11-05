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
		void updateState(const AppObject::Map & objectsVector, const int dt);
		static glm::vec4 getPlaneCoordinates(const glm::mat3 & matrix);
	};
}