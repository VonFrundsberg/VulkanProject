#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;
layout(location = 4) in vec4 inJoints;  // Joint indices
layout(location = 5) in vec4 inWeights; // Corresponding weights

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragUVCoord;

layout(set = 0, binding = 0) uniform GlobalUBO{
	mat4 projectionViewMatrix;
	vec3 directionToLight;
	mat4 u_jointMat[100];
} ubo;

layout(push_constant) uniform Push{
	mat4 modelMatrix; //projection * view * model
	mat4 normalMatrix; 
} push;

const float AMBIENT = 0.1;

void main()
{
	mat4 skinMatrix =
        inWeights.x * ubo.u_jointMat[int(inJoints.x)] +
        inWeights.y * ubo.u_jointMat[int(inJoints.y)] +
        inWeights.z * ubo.u_jointMat[int(inJoints.z)] +
        inWeights.w * ubo.u_jointMat[int(inJoints.w)];
	gl_Position = ubo.projectionViewMatrix * push.modelMatrix * skinMatrix * vec4(inPosition, 1.0);
	vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * inNormal);
	float lightIntensity = AMBIENT + max(dot(normalWorldSpace, ubo.directionToLight), 0);
	fragColor = lightIntensity * inColor;
	fragUVCoord = inUV;
}