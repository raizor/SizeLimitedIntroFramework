#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 index;

out vec4 pos;

void main(void)
{
	pos = position;
	gl_Position = position;
}