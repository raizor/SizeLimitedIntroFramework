#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;

void main(void)
{	
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);	
}