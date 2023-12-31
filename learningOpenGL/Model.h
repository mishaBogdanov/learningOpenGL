#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "Mesh.h"


class Model
{
private:
	std::vector<Mesh> mesh;
	bool load( std::string opening);
public:
	Model(std::string filepath);
	void Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam);

};

