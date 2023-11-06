#include "physicsSystem.hpp"
#include "physicsSystem.hpp"
#include "physicsSystem.hpp"
#include "physicsSystem.hpp"
#include <iostream>
#include <gtx/string_cast.hpp>

namespace appNamespace {
	PhysicsSystem::PhysicsSystem()
	{
	}
	void appNamespace::PhysicsSystem::updateState(AppObject::Map& objectsVector, const int dt)
	{
		for (auto& objectOf : objectsVector) {
			//const auto& objectOfInfo = objectOf.second;
			//std::cout << object.first << "\n";
			//const auto& objectOfPosition = objectOfInfo.transform.translation;
			//if (objectOfInfo.intersection.intersectionType == IntersectionComponent::ELLIPSE) {
				//const auto& objectSize = objectOfInfo.intersection.intersectionData;
			for (const auto& objectWith : objectsVector) {
				//if (objectOfInfo.intersection.intersectionType == IntersectionComponent::PLANE) {
				if (objectOf.first != objectWith.first) {
					const auto isIt = intersectionEngine.isIntersected(objectOf.second, objectWith.second);
					if (isIt != -1) {
						if (isIt == 0) {
							std::cout << "no intersection" << "\n";
							objectOf.second.isTarget = 0;
						}
						else if(isIt == 1) {
							std::cout << "intersection" << "\n";
							objectOf.second.isTarget = 1;
						}
						
					}
					
					
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
	glm::vec4 appNamespace::PhysicsSystem::getPlaneCoefficients(const glm::mat3& argMatrix)
	{
		const float d = 1.0f;
		glm::mat3 matrixD = argMatrix;
		const float D = glm::determinant(matrixD);
		//std::cout <<"d equals to: " << D << "\n";
		if (abs(D) < std::numeric_limits<float>::epsilon()) {
			//SORT OF EXCEPTION THAT HANDLES HORIZONTAL CASE AT ORIGIN
			return  glm::vec4{
				0.0f,
				1.0f,
				0.0f,
				0.0f};
		}
		glm::mat3 matrixA = argMatrix;
		matrixA[0][0] = 1.0f;
		matrixA[1][0] = 1.0f;
		matrixA[2][0] = 1.0f;

		glm::mat3 matrixB = argMatrix;
		matrixB[0][1] = 1.0f;
		matrixB[1][1] = 1.0f;
		matrixB[2][1] = 1.0f;

		glm::mat3 matrixC = argMatrix;
		matrixC[0][2] = 1.0f;
		matrixC[1][2] = 1.0f;
		matrixC[2][2] = 1.0f;

		const glm::vec4 equationCoeffs{
			-d / D * glm::determinant(matrixA),
			-d / D * glm::determinant(matrixB),
			-d / D * glm::determinant(matrixC), d };
		return equationCoeffs;
	}
	glm::vec4 appNamespace::PhysicsSystem::getPlaneCoefficientsGauss(const glm::mat3& argMatrix)
	{
		glm::mat3 matrix = argMatrix;

		const float d = 1.0f;
		glm::vec3 rhs{ d };
		glm::vec3 solution;
		const int n = 3;
		glm::mat3 permutation{ 1.0f };
		//std::cout << glm::to_string(permutation) << "\n";
		for (int i = 0; i < n; i++) {
			/////PERMUTATION
			int argMax = i;
			float max = matrix[i][i];
			for (int j = i + 1; j < n; j++) {
				if (abs(matrix[i][j]) >= abs(max)) {
					argMax = j;
					max = matrix[i][j];
				}
			}
			/*if (abs(max) < std::numeric_limits<float>::epsilon()) {
				std::cout << "we have a problem";
			}
			permutation[i][i] = 0;
			permutation[i][argMax] = 1;
			permutation[argMax][argMax] = 0;
			permutation[argMax][i] = 1;
			std::cout << "result after sweep \n" << glm::to_string(matrix) << "\n";
			matrix = permutation * matrix;
			std::cout << i << "'s iteration \n permutation matrix \n" << glm::to_string(permutation) << "\n";
			std::cout << "result after permutation \n" << glm::to_string(matrix) << "\n";*/
			////FORWARD SWEEP
			for (int j = i + 1; j < n; j++) {
				matrix[i][j] /= matrix[i][i];
			}
			rhs[i] /= matrix[i][i];
			matrix[i][i] = 1.0f;
			for (int j = i + 1; j < n; j++) {
				for (int k = i + 1; k < n; k++) {
					matrix[j][k] -= matrix[i][k] * matrix[j][i];
				}
				rhs[j] -= rhs[i] * matrix[j][i];
				matrix[j][i] = 0.0f;
			}
		}
		////BACKWARD SWEEP
		for (int i = n - 1; i >= 0; i--) {
			float sum = rhs[i];
			for (int j = i + 1; j < n; j++) {
				sum -= solution[j] * matrix[i][j];
			}
			solution[i] = sum;
		}
		return glm::vec4{ solution.x, solution.y, solution.z, d };
	}
}
