#pragma once

#include "ExperimentingWithShaders.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"
#include <stb/stb_image.h>
#include "TextureClass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <windows.h>
#include "Camera.h"
#include <vector>



class Mesh
{
public:
	std::vector <glm::vec3> vertices;
	std::vector <GLuint> indices;

	VAO vao;

	Mesh(std::vector <glm::vec3>& verticex, std::vector <GLuint>& indicies);

	void Draw(ShaderClass& shader, Camera& camera);
	void Draw(ShaderClass& shader, ShaderClass & shader2, Camera& camera);
};

