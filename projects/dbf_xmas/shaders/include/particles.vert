#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 index;

void main(void)
{
	gl_Position = position;
} 