#include "appCamera.hpp"
#include <cassert>
#include <limits>
#include <iostream>
namespace appNamespace{
    void AppCamera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
    {
        projectionMatrix = glm::mat4{ 1.0f };
        projectionMatrix[0][0] = 2.f / (right - left);
        projectionMatrix[1][1] = 2.f / (bottom - top);
        projectionMatrix[2][2] = 1.f / (far - near);
        projectionMatrix[3][0] = -(right + left) / (right - left);
        projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        projectionMatrix[3][2] = -near / (far - near);
    }

    void AppCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
    {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovy = tan(fovy / 2.f);
        projectionMatrix = glm::mat4{ 0.0f };
        projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
        projectionMatrix[1][1] = 1.f / (tanHalfFovy);
        projectionMatrix[2][2] = far / (far - near);
        projectionMatrix[2][3] = 1.f;
        projectionMatrix[3][2] = -(far * near) / (far - near);
    }
	void AppCamera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
	{
        const glm::vec3 w{ glm::normalize(direction) };
        const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
        const glm::vec3 v{ glm::cross(w, u) };

        viewMatrix = glm::mat4{ 1.f };
        viewMatrix[0][0] = u.x;
        viewMatrix[1][0] = u.y;
        viewMatrix[2][0] = u.z;
        viewMatrix[0][1] = v.x;
        viewMatrix[1][1] = v.y;
        viewMatrix[2][1] = v.z;
        viewMatrix[0][2] = w.x;
        viewMatrix[1][2] = w.y;
        viewMatrix[2][2] = w.z;
        viewMatrix[3][0] = -glm::dot(u, position);
        viewMatrix[3][1] = -glm::dot(v, position);
        viewMatrix[3][2] = -glm::dot(w, position);
	}
	void AppCamera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	{
        setViewDirection(position, target - position, up);
	}
    void helperCout(glm::vec3 vec, const std::string message) {
        std::cout << message;
        for (int i = 0; i < 3; i++) {
            switch (i) {
            case 0: {
                std::cout << vec.x << ", ";
                break;
            }
            case 1: {
                std::cout << vec.y << ", ";
                break;
            }
            case 2: {
                std::cout << vec.z << ", ";
                break;
            }
            }
        }
        std::cout << "\n";
    };
    void AppCamera::set3rdPersonCameraView(glm::vec3 relativePosition, glm::vec3 person,
        glm::vec3 rotation)
    {
        //rotation.x = 0;
        //rotation.y += +3.14;
        //rotation.z = 0;
        //const float rx = rotation.x - 0.5;
        /*const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);*/
        const float cx = glm::cos(rotation.x);
        const float sx = glm::sin(rotation.x);
        const float cy = glm::cos(rotation.y);
        const float sy = glm::sin(rotation.y);
        const glm::vec3 u{ (cy), sx*sy, cx*sy };
        const glm::vec3 v{0, cx, -sx };
        const glm::vec3 w{ -sy, cy * sx, cx * cy };
        /*viewMatrix = glm::mat4{ 1.f };
        viewMatrix[0][0] = u.x;
        viewMatrix[1][0] = u.y;
        viewMatrix[2][0] = u.z;
        viewMatrix[0][1] = v.x;
        viewMatrix[1][1] = v.y;
        viewMatrix[2][1] = v.z;
        viewMatrix[0][2] = w.x;
        viewMatrix[1][2] = w.y;
        viewMatrix[2][2] = w.z;*/
        //relativePosition.x = 1;
        //relativePosition.y = 1;
        //relativePosition.z = 1;
        //const glm::vec3 relativePositionRearrangement = { relativePosition.x, relativePosition.y,  relativePosition.z };
        /*const glm::vec3 rotatedRelativePosition = {
            glm::dot(u, relativePositionRearrangement),
            glm::dot(v, relativePositionRearrangement),
            glm::dot(w, relativePositionRearrangement) };*/
        /*const glm::vec3 rotatedRelativePosition = {
            glm::dot(u, relativePosition),
            glm::dot(v, relativePosition),
            glm::dot(w, relativePosition) };*/
        /*const float r = glm::dot(rotation, rotation);
        const glm::vec3 rotatedRelativePosition = {
            relativePosition * sin(rotation.x) * cos(rotation.y),
            relativePosition* sin(rotation.x) * sin(rotation.y),
            relativePosition* cos(rotation.x)};*/
        glm::vec3 rotatedRelativePosition = {
            glm::dot(u, relativePosition),
            glm::dot(v, relativePosition),
            glm::dot(w, relativePosition) };
        //viewMatrix[3][0] = -glm::dot(u, rotatedRelativePosition);
        //viewMatrix[3][1] = -glm::dot(v, rotatedRelativePosition);
        //viewMatrix[3][2] = -glm::dot(w, rotatedRelativePosition);
        //helperCout(rotation, "rotation ");
        //helperCout(relativePosition, "before ");
        //helperCout(rotatedRelativePosition, "unit vector ");
        /*viewMatrix[3][0] = -glm::dot(u, person + relativePosition);
        viewMatrix[3][1] = -glm::dot(v, person + relativePosition);
        viewMatrix[3][2] = -glm::dot(w, person + relativePosition);*/
        //c2 = glm::cos(rotation.x);
        //s2 = glm::sin(rotation.y);
        /*const glm::vec3 u2{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
        const glm::vec3 v2{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
        const glm::vec3 w2{ (c2 * s1), (-s2), (c1 * c2) };
        viewMatrix = glm::mat4{ 1.f };
        viewMatrix[0][0] = u2.x;
        viewMatrix[1][0] = u2.y;
        viewMatrix[2][0] = u2.z;
        viewMatrix[0][1] = v2.x;
        viewMatrix[1][1] = v2.y;
        viewMatrix[2][1] = v2.z;
        viewMatrix[0][2] = w2.x;
        viewMatrix[1][2] = w2.y;
        viewMatrix[2][2] = w2.z;
        viewMatrix[3][0] = -glm::dot(u, person + rotatedRelativePosition);
        viewMatrix[3][1] = -glm::dot(v, person + rotatedRelativePosition);
        viewMatrix[3][2] = -glm::dot(w, person + rotatedRelativePosition);*/
        //setViewYXZ(rotatedRelativePosition, rotation);
        const glm::vec3 relativePositionRearrangement = {
            rotatedRelativePosition.x,
            rotatedRelativePosition.y,
            rotatedRelativePosition.z };
        setViewTarget(relativePositionRearrangement + person, person);
    }
    
	void AppCamera::setViewYXZ(glm::vec3 position, glm::vec3 rotation)
	{
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
        const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
        const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
        viewMatrix = glm::mat4{ 1.f };
        viewMatrix[0][0] = u.x;
        viewMatrix[1][0] = u.y;
        viewMatrix[2][0] = u.z;
        viewMatrix[0][1] = v.x;
        viewMatrix[1][1] = v.y;
        viewMatrix[2][1] = v.z;
        viewMatrix[0][2] = w.x;
        viewMatrix[1][2] = w.y;
        viewMatrix[2][2] = w.z;
        viewMatrix[3][0] = -glm::dot(u, position);
        viewMatrix[3][1] = -glm::dot(v, position);
        viewMatrix[3][2] = -glm::dot(w, position);
	}
}
