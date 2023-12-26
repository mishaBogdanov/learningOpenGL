#version 330 core


layout (triangles) in;
layout (triangle_strip, max_vertices =3) out;



out vec4 givenColor;
out vec2 texture_coordinates;


in DATA
{
	vec2 texCoordinates;
	mat4 cameraMatrix;
} data_in[];

void main()
{
	gl_Position = data_in[0].cameraMatrix * gl_in[0].gl_position;
	givenColor = vec4(1.0, 1.0, 1.0, 1.0);
	texture_coordinates = data_in[0].texCoordinates;
	EmitVertex();

	gl_Position = data_in[1].cameraMatrix * gl_in[1].gl_position;
	texture_coordinates = data_in[1].texCoordinates;
	givenColor = vec4(1.0, 1.0, 1.0, 1.0);
	EmitVertex();

	gl_Position = data_in[2].cameraMatrix * gl_in[2].gl_position;
	texture_coordinates = data_in[2].texCoordinates;
	givenColor = vec4(1.0, 1.0, 1.0, 1.0);
	EmitVertex();

	EndPrimitive();
}

