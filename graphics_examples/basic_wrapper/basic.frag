// Minimal fragment shader

// Personal modification 420 to 410 core
#version 410 core

out vec4 outputColor;

// Personal modification for keyboard control
uniform vec3 color; // New: color control

void main()
{
    // Personal modification: Color control
    // vec4(0.0, 1.0, 1.0, 1.0) to vec4(color, 1.0)
	outputColor = vec4(color, 1.0);
}