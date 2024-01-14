#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "Mesh.h"
#include "Hitbox.h"

struct Contact {
	glm::vec3 position;
	glm::vec3 normal;
};

struct IntersectionModel {
	Model* model1;
	Model* model2;
	float amountIntersect;
	glm::vec3 normal;
};

struct Impulse {
	glm::vec3 direction;
	glm::vec3 position;
};


class Model
{
private:
	std::vector<Mesh> mesh;
	//glm::vec3 pos;
	glm::mat4 translation;
	glm::mat4 inverseTranslation;

	glm::mat4 inertiaTensor;



	glm::vec3 facing;
	glm::vec3 up;
	glm::vec3 velocity;
	glm::vec3 angularVelocityDirection;
	double angularVelocity;
	std::vector<ShaderClass> shaders;
	bool movable;
	std::vector<float> wallsDistance;


	std::vector<glm::vec3> corners;
	std::vector<glm::vec3> originalCorners;
	glm::vec3 cm;
	glm::vec3 originalCm;
	std::vector<glm::vec3> hitboxVectors;

	std::vector<Hitbox> hitboxes;


	std::vector<Impulse> impulses;
	//float maxDistFromCm;
	std::vector<float> distToCenter;


	float bounceFactor;
	float mass;
	float frictionFactor;

	void setupModel(float mass);
	void setupCybertruck(float scale);
	bool load(std::string opening, float scale, bool customLocation, glm::vec3 newLocation);

	
public:
	Model(std::string filepath);

	Model(std::string filepath, float scale);
	Model(std::string filepath, glm::vec3 location);
	Model(std::string filepath, float scale, glm::vec3 location);
	Model(float length, float gheight, float gz, float gx);


	void Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam);
	void Draw(Camera cam);
	glm::mat4 getTransformation();
	glm::mat4 getInverseTransformation();


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

	void getHitboxes(Hitbox * & firstHitbox, int & size);
	int getHitboxesSize();

	void addIntersection(IntersectionModel * given);
	Hitbox* getHitbox(int i);

	bool isMovable();
	void handleCollisions();

	void changeIsMovable(bool value);

	void addImpulse(Impulse imp);
	void dealWithImpulses();
	float getBounceFactor();
	float getFrictionFactor();
	glm::vec3* getAngularVelocity();
};

