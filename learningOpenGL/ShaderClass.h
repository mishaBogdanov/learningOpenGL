#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
std::string get_file_contents(const char* filename);
class ShaderClass
{
public:
	GLuint ID;
	ShaderClass( char* vertexFile,  char* fragmentFile);

	void Activate();
	void Delete();

};



