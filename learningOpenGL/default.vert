#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 coordinates;

out vec4 return_color;
out vec2 texture_coordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


uniform float scale;


void main()
{
	gl_Position = proj * view * model * vec4(aPos*(scale*2+0.5), 1.0);
	return_color=vec4(color, 1.0f);
	texture_coordinates = coordinates;
}