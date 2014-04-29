#version 330

layout(location=0) in vec4 inPosition;
layout(location=1) in vec4 inColor;
layout(location=2) in vec3 inNormal;

out Data {
	vec3 normal;
	vec4 eye;
	vec4 color;
} dataOut;

// uniform block for default matrices
//layout (std140) uniform Matrices {
//	mat4 modelViewMatrix;
//	mat4 projectionMatrix;
//}

struct Matrices {
	mat4 modelViewMatrix;
	mat3 normalMatrix; // inverse transpose mv
	mat4 projectionMatrix;
};
uniform Matrices matrices;

void main(void) {

	dataOut.normal = matrices.normalMatrix * inNormal;
	dataOut.eye = matrices.modelViewMatrix * inPosition;
    gl_Position = (matrices.projectionMatrix * matrices.modelViewMatrix) * inPosition;
    dataOut.color = inColor;
}