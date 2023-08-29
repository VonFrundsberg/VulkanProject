#version 450



layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUVCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D texSampler;

layout(push_constant) uniform Push{
	mat4 modelMatrix; //projection * view * model
	mat4 normalMatrix; 
} push;

void main(){
	if (push.normalMatrix[3][3] <= 0.0){
		//outColor = texture(texSampler, fragUVCoord) * vec4(fragColor, 1.0);
		outColor = texture(texSampler, fragUVCoord);
	}
	else{
		outColor = vec4(texture(texSampler, fragUVCoord).x, 0.0, 0.0, 1.0);
	}
}