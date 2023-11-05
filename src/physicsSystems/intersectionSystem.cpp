#include "intersectionSystem.hpp"
#include "physicsSystem.hpp"
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <gtx/string_cast.hpp>
appNamespace::IntersectionSystem::IntersectionSystem()
{
}

int appNamespace::IntersectionSystem::isIntersected(const AppObject& objectOf, const AppObject& objectWith)
{
	if (objectOf.intersection.intersectionType == IntersectionComponent::PLANE) {
		return -1;
	}
	else if(objectWith.intersection.intersectionType == IntersectionComponent::PLANE) {
		const auto& planeTranslation = objectWith.transform.translation;
		const auto& planeSize = objectWith.intersection.intersectionData;
		std::cout << "plane data \n";
		for (const auto& number : planeSize) {
			std::cout << number << ", ";
		}
		glm::mat3 matrix;
		std::memcpy(glm::value_ptr(matrix),	planeSize.data(), 9 * sizeof(float));
		PhysicsSystem::getPlaneCoordinates(matrix);
		std::cout << "\n";
	}
}
