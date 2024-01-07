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
	bool load( std::string opening, float scale, bool customLocation, glm::vec3 newLocation);
	//glm::vec3 pos;
	glm::mat4 translation;



	glm::vec3 facing;
	glm::vec3 up;
	glm::vec3 velocity;
	glm::vec3 angularVelocityDirection;
	double angularVelocity;
	std::vector<ShaderClass> shaders;
	bool movable;

	std::vector<glm::vec3> corners;
	std::vector<glm::vec3> originalCorners;
	glm::vec3 cm;
	glm::vec3 originalCm;
	std::vector<glm::vec3> hitboxVectors;
	float mass;

	void setupModel();
	
public:
	Model(std::string filepath);

	Model(std::string filepath, float scale);
	Model(std::string filepath, glm::vec3 location);
	Model(std::string filepath, float scale, glm::vec3 location);
	Model(float length, float gheight, float gz, float gx);


	void Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam);
	void Draw(Camera cam);
	glm::mat4 getTransformation();

	void update(float deltaT);
	void setVelocity(glm::vec3 &givenV);
	void scaleVelocity(float scale);
	void setPosition(glm::vec3 givenPos);
	void getHitboxCorners(std::vector<glm::vec3>& returning);
	void getMaxMinFromProjection(glm::vec3& projectVec, float& max, float& min);
	void rotate(float angle, glm::vec3& norm);
	void moveBy(glm::vec3& given);
	float getMass();
	glm::vec3* getCm();
	void addAngularVelocity(glm::vec3& givenVelocity);
	float getMagnitudeVelocity();
	glm::vec3 getVelocity();
	void addVelocity(glm::vec3& given);
	glm::vec3 getFacing();

	std::vector<glm::vec3> getHitboxVectors();
	bool isMovable();
};

