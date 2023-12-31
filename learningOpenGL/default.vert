#version 330 core
layout (location = 0) in vec3 aPos;


uniform mat4 camMatrix;
uniform mat4 translationMatrix;



out DATA
{
	mat4 cameraMatrix;
	mat4 given_translation_matrix;
} data_out;


void main()
{
	gl_Position = vec4(aPos, 1.0);
	
	data_out.cameraMatrix = camMatrix;
	data_out.given_translation_matrix = translationMatrix;
}