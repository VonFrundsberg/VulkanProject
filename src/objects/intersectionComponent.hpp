#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace appNamespace {
	class IntersectionComponent {
	public:
		static const int ELLIPSE = 0;
		static const int PLANE = 1;
		
		int intersectionType{};
		std::vector<float> intersectionData{};
		IntersectionComponent() {};
		IntersectionComponent(const int intersectionTypeArg, const std::vector<float> & intersectionDataArg);

		void setComponentData(const int intersectionTypeArg, const std::vector<float>& intersectionDataArg);
	};
}