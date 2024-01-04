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
	bool load( std::string opening, float scale);
	glm::vec3 pos;
	glm::mat4 translation;



	glm::vec3 facing;
	glm::vec3 up;
	glm::vec3 velocity;
	glm::vec3 angularVelocityDirection;
	double angularVelocity;
	std::vector<ShaderClass> shaders;

	glm::vec3 corners[8];
	glm::vec3 cm;

	void setupModel();
public:
	Model(std::string filepath);
	Model(std::string filepath, float scale);
	void Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam);
	void Draw(Camera cam);
	glm::mat4 getTransformation();

	void update(float deltaT);
	void setVelocity(glm::vec3 &givenV);
	void scaleVelocity(float scale);
	void setPosition(glm::vec3 givenPos);
};

