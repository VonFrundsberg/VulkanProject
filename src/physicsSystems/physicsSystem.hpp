#pragma once

#include "intersectionSystem.hpp"

namespace appNamespace {
	class PhysicsSystem {
	public:
		intersectionSystem intersectionEngine;
		PhysicsSystem();
		updateState(const int dt) {};
	};
}