#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

void VAO::LingVBO(VBO VBO, GLuint layout) {
	VBO.Bind();
	//glVertexA
}