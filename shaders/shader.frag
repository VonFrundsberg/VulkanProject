#version 450

//layout(binding = 1) uniform sampler2D texSampler;


layout(push_constant) uniform Push{
	vec3 offset;
	vec3 color;
} push;
//layout(location = 0) in vec3 fragColor;
//layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main(){
	//outColor = texture(texSampler, fragTexCoord);
	outColor = vec4(push.color, 1.0);
}