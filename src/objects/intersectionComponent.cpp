#include <intersectionComponent.hpp>

namespace appNamespace {
	IntersectionComponent::IntersectionComponent(const int intersectionTypeArg,
		const std::vector<float>& intersectionDataArg){
		this->intersectionType = intersectionTypeArg;
		this->intersectionData = intersectionDataArg;
	}
	void IntersectionComponent::setComponentData(const int intersectionTypeArg, const std::vector<float>& intersectionDataArg)
	{
		this->intersectionType = intersectionTypeArg;
		this->intersectionData = intersectionDataArg;
	}
}