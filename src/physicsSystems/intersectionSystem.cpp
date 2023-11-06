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
		/*std::cout << "plane data \n";
		for (const auto& number : planeSize) {
			std::cout << number << ", ";
		}*/
		glm::mat3 matrix;
		std::memcpy(glm::value_ptr(matrix),	planeSize.data(), 9 * sizeof(float));
		const glm::vec4 planeCoeffs = PhysicsSystem::getPlaneCoefficients(matrix);
		const glm::vec3 objectOfTranslation = objectOf.transform.translation;
		const float d = planeCoeffs.w 
			+ objectOfTranslation.x * planeCoeffs.x
			+ objectOfTranslation.y * planeCoeffs.y
			+ objectOfTranslation.z * planeCoeffs.z;
		//std::cout << glm::to_string(planeCoeffs) << "\n";
		//std::cout << glm::to_string(objectOfTranslation) << "\n";
		const float distance = abs(d) / sqrt(
			planeCoeffs.x * planeCoeffs.x +
			planeCoeffs.y * planeCoeffs.y +
			planeCoeffs.z * planeCoeffs.z);
		//std::cout << "distance is: " << distance << "\n";
		//std::cout << "sqrt of plane coeffs " << sqrt(glm::dot(planeCoeffs, planeCoeffs)) << "\n";
		if (distance < objectOf.intersection.intersectionData[0]) {
			return 1;
		}
		else {
			return 0;
		}
	}
}
