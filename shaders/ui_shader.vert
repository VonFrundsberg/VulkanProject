#version 450

const vec2 positions[3] = vec2[](
  vec2(0.0, -0.5),
  vec2(0.5, 0.5),
  vec2(-0.5, 0.5)
);

layout(set = 0, binding = 0) uniform GlobalUBO{
	mat4 projectionViewMatrix;
	vec3 directionToLight;
} ubo;

void main()
{
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}