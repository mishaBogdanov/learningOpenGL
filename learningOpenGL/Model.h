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
	glm::vec3 pos;
	glm::mat4 translation;
	glm::vec3 facing;
	glm::vec3 up;
	glm::vec3 velocity;
	glm::vec3 angularVelocityDirection;
	double angularVelocity;
	std::vector<ShaderClass> shaders;
public:
	Model(std::string filepath);
	void Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam);
	void Draw(Camera cam);

	void update(float deltaT);
	void setVelocity(glm::vec3 &givenV);
	void scaleVelocity(float scale);
};

