#pragma once
#include "../objects/appObject.hpp"

namespace appNamespace {
	class IntersectionSystem {
	public:
		IntersectionSystem();
		int isIntersected(const AppObject &objectOf, const AppObject &objectWith);
	};
}