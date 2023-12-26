#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 coordinates;


uniform mat4 camMatrix;


uniform float scale;

out DATA
{
	vec2 texCoordinates;
	mat4 cameraMatrix;
} data_out;


void main()
{
	gl_Position = vec4(aPos, 1.0);
	
	data_out.texCoordinates = coordinates;
	data_out.cameraMatrix = camMatrix;
}