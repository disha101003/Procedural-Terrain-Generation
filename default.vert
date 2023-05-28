
#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 asColor;

out vec3 color;

uniform mat4 camMat;

void main()
{
	gl_Position = camMat * vec4(Pos, 1.0);
	color = asColor;
}