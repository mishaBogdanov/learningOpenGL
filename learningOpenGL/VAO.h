#pragma once

#include<glad/glad.h>
#include"VBO.h"
class VAO
{
public:
	GLuint ID;
	VAO();

	void LingVBO(VBO VBO, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();


};

