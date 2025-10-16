// Minimal vertex shader

// Personal modification 420 to 410 core
#version 410 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}