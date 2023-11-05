#include "physicsSystem.hpp"
#include "physicsSystem.hpp"
#include "physicsSystem.hpp"
#include <iostream>
#include <gtx/string_cast.hpp>

namespace appNamespace {
	PhysicsSystem::PhysicsSystem()
	{
	}
	void appNamespace::PhysicsSystem::updateState(const AppObject::Map& objectsVector, const int dt)
	{
		for (const auto& objectOf : objectsVector) {
			//const auto& objectOfInfo = objectOf.second;
			//std::cout << object.first << "\n";
			//const auto& objectOfPosition = objectOfInfo.transform.translation;
			//if (objectOfInfo.intersection.intersectionType == IntersectionComponent::ELLIPSE) {
				//const auto& objectSize = objectOfInfo.intersection.intersectionData;
			for (const auto& objectWith : objectsVector) {
				//if (objectOfInfo.intersection.intersectionType == IntersectionComponent::PLANE) {
				if (objectOf.first != objectWith.first) {
					const auto isIt = intersectionEngine.isIntersected(objectOf.second, objectWith.second);
					std::cout << "is intersected: " << isIt << "\n";
					
				}
			//}
			}
			//const auto& object
			//std::cout << glm::to_string(objectInfo.transform.translation) << "\n";
			//std::cout << objectInfo.intersection.intersectionType << "\n";
			//for(const auto)
			//std::cout << objectInfo.intersection.intersectionData << "\n";
		}
	}
	glm::vec4 appNamespace::PhysicsSystem::getPlaneCoordinates(const glm::mat3 & argMatrix)
	{
		glm::mat3 matrix = argMatrix;
		glm::vec3 rhs{ 1.0f };
		for (int i = 0; i < 3; i++) {
			for (int j = i + 1; j < 3; j++) {
				matrix[i][j] /= matrix[i][i];
				
			}
			rhs[i] /= matrix[i][i];
			matrix[i][i] = 1.0f;

			std::cout << "matrix: \n" << glm::to_string(matrix) << "\n";

			for (int j = i + 1; j < 3; j++) {
				for (int k = i; k < 3; k++) {
					matrix[j][k] -= matrix[i][k] * matrix[i][j];
				}
				rhs[j] -= rhs[i] * matrix[i][j];
				matrix[j][i] = 0.0f;
			}
			std::cout << "matrix: \n" << glm::to_string(matrix) << "\n";
		}
		return glm::vec4();
	}
}
