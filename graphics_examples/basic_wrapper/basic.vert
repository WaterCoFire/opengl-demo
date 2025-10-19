// Minimal vertex shader

// Personal modification 420 to 410 core
#version 410 core

layout(location = 0) in vec4 position;

// Personal modification for keyboard control
uniform vec2 offset;

void main()
{
    // Personal modification: + vec4(offset, 0.0, 0.0)
	gl_Position = position + vec4(offset, 0.0, 0.0);
}