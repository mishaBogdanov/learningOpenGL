#include "Model.h"
#pragma warning(disable:4996)
#include "Mesh.h"
#include <iostream>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>
#include "MyMath.h"





Model::Model(std::string location) {
	load(location, 1, false, glm::vec3(0, 0, 0));
	setupModel();
}

void Model::setupModel() {
	mass = 100;
	translation = glm::mat4(1);
	velocity = glm::vec3(0, 0, 0);
	angularVelocityDirection = glm::vec3(0, 0, 0);
	angularVelocity = 0;
	facing = glm::vec3(0, 0, 1);
	ShaderClass shaderProgram("default.vert", "default.geom", "default.frag");
	ShaderClass shaderProgram2("default.vert", "outline.geom", "outline.frag");
	shaders.push_back(shaderProgram);
	shaders.push_back(shaderProgram2);
	for (int k = 0; k < shaders.size(); k++) {
		shaders[k].Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaders[k].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(getTransformation()));
	}
	hitboxVectors.push_back(glm::vec3(1, 0, 0));
	hitboxVectors.push_back(glm::vec3(0, 1, 0));
	hitboxVectors.push_back(glm::vec3(0, 0, 1));
	movable = true;
	std::vector<int> edges = { 0,1,2,3,4,5,6,7,0,4,0,2,1,5,1,3,2,6,3,7,4,6,5,7 };

	Hitbox h = Hitbox(originalCorners, hitboxVectors, this, edges);
	hitboxes.push_back(h);
	bounceFactor = 0.5f;
	frictionFactor = 0.5f;
}




Model::Model(std::string location, float scale) {
	load(location, scale, false, glm::vec3(0, 0, 0));
	setupModel();
}

Model::Model(std::string location, glm::vec3 physLocation) {
	load(location, 1, true, physLocation);
	setupModel();
}

Model::Model(float length, float gheight, float gz, float gx) {
	angularVelocity = 0;
	angularVelocityDirection = glm::vec3(0);
	cm = glm::vec3(gx, gheight, gz);
	facing = glm::vec3(0, 1, 0);
	mass = 100;
	originalCm = cm;
	translation = glm::mat4(1);
	up = glm::vec3(0, 1, 0);
	velocity = glm::vec3(0, 0, 0);
	hitboxVectors.push_back(glm::vec3(0, 1, 0));
	corners.push_back(glm::vec3(gx - length / 2, gheight, gz - length / 2));
	corners.push_back(glm::vec3(gx - length / 2, gheight, gz + length / 2));
	corners.push_back(glm::vec3(gx + length / 2, gheight, gz - length / 2));
	corners.push_back(glm::vec3(gx + length / 2, gheight, gz + length / 2));


	movable = false;
}


Model::Model(std::string location, float scale, glm::vec3 physLocation) {
	load(location, scale, true, physLocation);
	setupModel();
}





void Model::Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam) {
	for (int i = 0; i < mesh.size(); i++) {
		mesh[i].Draw(shader, cam);
		mesh[i].Draw(shader2, cam);
	}
}

glm::mat4 Model::getTransformation() {
	return glm::translate(glm::mat4(1), cm) * translation * glm::translate(glm::mat4(1), -originalCm);
}


void Model::Draw(Camera cam) {
	for (int i = 0; i < mesh.size(); i++) {
		for (int k = 0; k < shaders.size(); k++) {
			mesh[i].Draw(shaders[k], cam);
		}

	}
}

void Model::update(float deltaT) {
	cm = cm + velocity * deltaT;

	if (angularVelocityDirection.x != 0 || angularVelocityDirection.y != 0 || angularVelocityDirection.z != 0) {
		translation = glm::rotate(translation, sqrt(MyMath::getVectorMagnitudeSquared(angularVelocityDirection) * deltaT), angularVelocityDirection);
	}

	if (MyMath::getVectorMagnitudeSquared(angularVelocityDirection) < 0.0000000001) {
		angularVelocityDirection = glm::vec3(0);
	}
	else {
		angularVelocityDirection = angularVelocityDirection * 0.99f * (1 - deltaT);
	}


	//for (int i = 0; i < corners.size(); i++) {
	//	corners[i] = getTransformation() * glm::vec4(originalCorners[i], 1);
	//}
	for (int i = 0; i < hitboxes.size(); i++) {
		glm::mat4 full = getTransformation();
		hitboxes[i].update(&full, &translation);
	}


	for (int k = 0; k < shaders.size(); k++) {
		shaders[k].Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaders[k].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(getTransformation()));
	}
}

void Model::setVelocity(glm::vec3& v) {
	velocity = v;
}

void Model::scaleVelocity(float g) {
	velocity *= g;
}

std::vector<glm::vec3> Model::getHitboxVectors() {
	std::vector<glm::vec3> returning;
	for (int i = 0; i < hitboxVectors.size(); i++) {
		returning.push_back(translation * glm::vec4(hitboxVectors[i], 1));
	}
	return returning;
}

void Model::getHitboxes(Hitbox*& firstHitbox, int& size) {
	firstHitbox = &hitboxes[0];
	size = hitboxes.size();
}

int Model::getHitboxesSize() {
	return hitboxes.size();
}

void Model::getHitboxCorners(std::vector<glm::vec3>& returning) {
	for (int i = 0; i < corners.size(); i++) {
		returning.push_back(corners[i]);
	}
}

void Model::getMaxMinFromProjection(glm::vec3& projectVec, float& max, float& min) {
	bool isFirst = true;
	for (int i = 0; i < corners.size(); i++) {
		glm::vec3 val;
		MyMath::projection(projectVec, corners[i], val);
		float square = MyMath::getVecMultiple(projectVec, val);
		if (isFirst) {
			max = square;
			min = square;
			isFirst = false;
		}
		else if (square > max) {
			max = square;
		}
		else if (square < min) {
			min = square;
		}
	}
}



bool Model::load(std::string given, float scale, bool customLocation, glm::vec3 newLocation) {


	std::ifstream file(given);
	std::vector<glm::vec3> Vertices = {};
	std::vector<GLuint> Indices = {};
	int currentVertexesNumber = 0;
	int lastMaxVertexNumber = 1;

	float PosX, PosY, PosZ, NegX, NegY, NegZ;

	bool isFirst = true;

	if (file.is_open()) {
		std::istream_iterator<std::string> fileIterator(file);
		std::istream_iterator<std::string> endIterator;

		while (fileIterator != endIterator) {
			if ((*fileIterator) == "v") {

				currentVertexesNumber += 1;
				++fileIterator;
				float val1 = std::stof((*fileIterator)) * scale;
				++fileIterator;
				float val2 = std::stof((*fileIterator)) * scale;
				++fileIterator;
				float val3 = std::stof((*fileIterator)) * scale;
				if (isFirst) {
					PosX = val1;
					NegX = val1;
					PosY = val2;
					NegY = val2;
					PosZ = val3;
					NegZ = val3;
					isFirst = false;
				}
				if (val1 > PosX) {
					PosX = val1;
				}
				if (val2 > PosY) {
					PosY = val2;
				}
				if (val3 > PosZ) {
					PosZ = val3;
				}
				if (val1 < NegX) {
					NegX = val1;
				}
				if (val2 < NegY) {
					NegY = val2;
				}
				if (val3 < NegZ) {
					NegZ = val3;
				}
				Vertices.push_back(glm::vec3(val1, val2, val3));
			}
			else if ((*fileIterator) == "f") {
				int vertices[3] = { 0,0,0 };
				for (int k = 0; k < 3; k++) {
					++fileIterator;
					char curChar = ' ';
					std::string current = "";
					int i = 0;
					while (curChar != '/') {
						curChar = (*fileIterator)[i];
						current += curChar;
						i++;
					}
					vertices[k] = std::stoi(current) - lastMaxVertexNumber;
				}

				Indices.push_back(vertices[0]);
				Indices.push_back(vertices[1]);
				Indices.push_back(vertices[2]);
			}
			else if ((*fileIterator) == "o") {
				lastMaxVertexNumber = currentVertexesNumber + 1;
				mesh.push_back(Mesh(Vertices, Indices));
				Vertices.clear();
				Indices.clear();
			}


			++fileIterator;

		}
		lastMaxVertexNumber = currentVertexesNumber;
		mesh.push_back(Mesh(Vertices, Indices));
		Vertices.clear();
		Indices.clear();
		//maxDistFromCm = sqrt(PosX * PosX + PosY * PosY + PosZ * PosZ);
		//if (sqrt(PosX * PosX + PosY * PosY + NegZ * NegZ) > maxDistFromCm) {
		//	maxDistFromCm = sqrt(PosX * PosX + PosY * PosY + NegZ * NegZ);
		//}if (sqrt(PosX * PosX + NegY * NegY + PosZ * PosZ) > maxDistFromCm) {
		//	maxDistFromCm = sqrt(PosX * PosX + NegY * NegY + PosZ * PosZ);
		//}if (sqrt(PosX * PosX + NegY * NegY + NegZ * NegZ) > maxDistFromCm) {
		//	maxDistFromCm = sqrt(PosX * PosX + NegY * NegY + NegZ * NegZ);
		//}if (sqrt(PosX * PosX + PosY * PosY + PosZ * PosZ) > maxDistFromCm) {
		//	maxDistFromCm = sqrt(NegX*NegX + PosY * PosY + PosZ * PosZ);
		//}if (sqrt(NegX * NegX + PosY * PosY + NegZ * NegZ) > maxDistFromCm) {
		//	maxDistFromCm = sqrt(NegX * NegX + PosY * PosY + NegZ * NegZ);
		//}if (sqrt(NegX * NegX + NegY * NegY + PosZ * PosZ) > maxDistFromCm) {
		//	maxDistFromCm = sqrt(NegX * NegX + NegY * NegY + PosZ * PosZ);
		//}if (sqrt(NegX * NegX + NegY * NegY + NegZ * NegZ) > maxDistFromCm) {
		//	maxDistFromCm = sqrt(NegX * NegX + NegY * NegY + NegZ * NegZ);
		//}
		distToCenter.push_back(PosX);
		distToCenter.push_back(NegX);
		distToCenter.push_back(PosY);
		distToCenter.push_back(NegY);
		distToCenter.push_back(PosZ);
		distToCenter.push_back(NegZ);


		originalCorners.push_back(glm::vec3(PosX, PosY, PosZ));
		originalCorners.push_back(glm::vec3(PosX, PosY, NegZ));
		originalCorners.push_back(glm::vec3(PosX, NegY, PosZ));
		originalCorners.push_back(glm::vec3(PosX, NegY, NegZ));
		originalCorners.push_back(glm::vec3(NegX, PosY, PosZ));
		originalCorners.push_back(glm::vec3(NegX, PosY, NegZ));
		originalCorners.push_back(glm::vec3(NegX, NegY, PosZ));
		originalCorners.push_back(glm::vec3(NegX, NegY, NegZ));
		for (int i = 0; i < originalCorners.size(); i++) {
			corners.push_back(originalCorners[i]);
		}




		glm::vec3 currentSum = corners[0];
		for (int i = 1; i < corners.size(); i++) {
			currentSum += corners[i];
		}
		cm = currentSum / (float)corners.size();
		originalCm = cm;
		if (customLocation) {
			cm = newLocation;

		}

		file.close();


	}

	return 0;
}


void Model::rotate(float angle, glm::vec3& norm) {
	glm::mat4 rot = glm::mat4(1);
	rot = glm::rotate(rot, glm::radians(angle), norm);
	translation = rot * translation;
}

void Model::moveBy(glm::vec3& given) {
	cm += given;
}

float Model::getMass() {
	return mass;
}

glm::vec3* Model::getCm() {
	return &cm;
}

void Model::addAngularVelocity(glm::vec3& given) {
	angularVelocityDirection += given;
}

float Model::getMagnitudeVelocity() {
	return sqrt(MyMath::getVectorMagnitudeSquared(velocity));
}

glm::vec3 Model::getVelocity() {
	return velocity;
}

void Model::addVelocity(glm::vec3& given) {
	velocity += given;
}

glm::vec3 Model::getFacing() {
	return glm::vec4(facing, 1) * getTransformation();
}

bool Model::isMovable() {
	return movable;
}


void Model::setupCybertruck(float scale) {

	std::vector<glm::vec3> frontLeftWheel;
	std::vector<glm::vec3> frontRightWheel;
	std::vector<glm::vec3> rearRightWheel;
	std::vector<glm::vec3> rearLeftWheel;
	std::vector<glm::vec3> body;
	std::vector<glm::vec3> wheelNormal;
	std::vector<glm::vec3> bodyNormal;




	frontRightWheel.push_back(glm::vec3(-1.068526, -0.024803, 2.109123) * scale);
	frontRightWheel.push_back(glm::vec3(-1.068526, 0.377191, 2.456192) * scale);
	frontRightWheel.push_back(glm::vec3(-1.068526, -0.024803, 1.828386) * scale);
	frontRightWheel.push_back(glm::vec3(-1.068526, 0.377191, 1.481317) * scale);
	frontRightWheel.push_back(glm::vec3(-0.682371, -0.024803, 2.109123) * scale);
	frontRightWheel.push_back(glm::vec3(-0.682371, 0.377191, 2.456192) * scale);
	frontRightWheel.push_back(glm::vec3(-0.682371, -0.024803, 1.828386) * scale);
	frontRightWheel.push_back(glm::vec3(-0.682371, 0.377191, 1.481317) * scale);

	frontLeftWheel.push_back(glm::vec3(0.686718, -0.024803, 2.109123) * scale);
	frontLeftWheel.push_back(glm::vec3(0.686718, 0.377191, 2.456192) * scale);
	frontLeftWheel.push_back(glm::vec3(0.686718, -0.024803, 1.828386) * scale);
	frontLeftWheel.push_back(glm::vec3(0.686718, 0.377191, 1.481317) * scale);
	frontLeftWheel.push_back(glm::vec3(1.072873, -0.024803, 2.109123) * scale);
	frontLeftWheel.push_back(glm::vec3(1.072873, 0.377191, 2.456192) * scale);
	frontLeftWheel.push_back(glm::vec3(1.072873, -0.024803, 1.828386) * scale);
	frontLeftWheel.push_back(glm::vec3(1.072873, 0.377191, 1.481317) * scale);

	rearLeftWheel.push_back(glm::vec3(0.686718, -0.024803, -1.618060) * scale);
	rearLeftWheel.push_back(glm::vec3(0.686718, 0.377191, -1.270991) * scale);
	rearLeftWheel.push_back(glm::vec3(0.686718, -0.024803, -1.898797) * scale);
	rearLeftWheel.push_back(glm::vec3(0.686718, 0.377191, -2.245865) * scale);
	rearLeftWheel.push_back(glm::vec3(1.072873, -0.024803, -1.618060) * scale);
	rearLeftWheel.push_back(glm::vec3(1.072873, 0.377191, -1.270991) * scale);
	rearLeftWheel.push_back(glm::vec3(1.072873, -0.024803, -1.898797) * scale);
	rearLeftWheel.push_back(glm::vec3(1.072873, 0.377191, -2.245865) * scale);

	rearRightWheel.push_back(glm::vec3(-1.068526, -0.024803, -1.618060) * scale);
	rearRightWheel.push_back(glm::vec3(-1.068526, 0.377191, -1.270991) * scale);
	rearRightWheel.push_back(glm::vec3(-1.068526, -0.024803, -1.898797) * scale);
	rearRightWheel.push_back(glm::vec3(-1.068526, 0.377191, -2.245865) * scale);
	rearRightWheel.push_back(glm::vec3(-0.682371, -0.024803, -1.618060) * scale);
	rearRightWheel.push_back(glm::vec3(-0.682371, 0.377191, -1.270991) * scale);
	rearRightWheel.push_back(glm::vec3(-0.682371, -0.024803, -1.898797) * scale);
	rearRightWheel.push_back(glm::vec3(-0.682371, 0.377191, -2.245865) * scale);

	body.push_back(glm::vec3(-1.068788, 0.364160, 2.884884) * scale);
	body.push_back(glm::vec3(-1.068788, 1.871332, 0.464945) * scale);
	body.push_back(glm::vec3(-1.068788, 0.364160, -2.808581) * scale);
	body.push_back(glm::vec3(-1.068788, 1.871332, -2.808581) * scale);
	body.push_back(glm::vec3(1.059664, 0.364160, 2.884884) * scale);
	body.push_back(glm::vec3(1.059664, 1.871332, 0.464945) * scale);
	body.push_back(glm::vec3(1.059664, 0.364160, -2.808581) * scale);
	body.push_back(glm::vec3(1.059664, 1.871332, -2.808581) * scale);
	body.push_back(glm::vec3(-1.068788, 1.021972, 2.884884) * scale);
	body.push_back(glm::vec3(-1.068788, 1.021972, -2.808581) * scale);
	body.push_back(glm::vec3(1.059664, 1.021972, -2.808581) * scale);
	body.push_back(glm::vec3(1.059664, 1.021972, 2.884884) * scale);

	wheelNormal.push_back(glm::vec3(0, 1, 0));
	wheelNormal.push_back(glm::vec3(1, 0, 0));
	wheelNormal.push_back(glm::vec3(0, -1 / sqrt(2), 1 / sqrt(2)));
	wheelNormal.push_back(glm::vec3(0, -1 / sqrt(2), -1 / sqrt(2)));


	bodyNormal.push_back(glm::vec3(0, 1, 0));
	bodyNormal.push_back(glm::vec3(1, 0, 0));
	bodyNormal.push_back(glm::vec3(0, 0, 1));
	bodyNormal.push_back(glm::vec3(0, 1 / sqrt(8.1175480847), 2.84913111048 / sqrt(8.1175480847)));
}



Hitbox* Model::getHitbox(int i) {
	return &hitboxes[i];
}


void Model::changeIsMovable(bool newValue) {
	movable = newValue;
}

void Model::addImpulse(Impulse imp) {
	impulses.push_back(imp);
}

float Model::getBounceFactor() {
	return bounceFactor;
}

float Model::getFrictionFactor() {
	return frictionFactor;
}

glm::vec3* Model::getAngularVelocity() {
	return &angularVelocityDirection;
}

void Model::dealWithImpulses() {
	for (int i = 0; i < impulses.size(); i++) {
		glm::vec3 cmToImpulse = impulses[i].position - cm;
		cmToImpulse.x = cmToImpulse.x / distToCenter[0];
		cmToImpulse.y = cmToImpulse.y / distToCenter[2];
		cmToImpulse.z = cmToImpulse.z / distToCenter[4];

		

		cmToImpulse = glm::normalize(cmToImpulse);
		velocity += glm::dot(cmToImpulse, glm::normalize( - impulses[i].direction)) * impulses[i].direction;
		angularVelocityDirection += glm::cross(cmToImpulse, impulses[i].direction)/ 30.0f;


	}
	impulses.clear();
}