#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 coordinates;

out vec4 return_color;
out vec2 texture_coordinates;

uniform float scale;


void main()
{
	gl_Position = vec4(aPos.x * scale, aPos.y * scale, aPos.z * scale, 1.0);
	return_color=vec4(color, 1.0f);
	texture_coordinates = coordinates;

}