// Minimal fragment shader

// Personal modification 420 to 410 core

#version 410 core

in vec4 fcolour;
out vec4 outputColor;
void main()
{
	outputColor = fcolour;
}