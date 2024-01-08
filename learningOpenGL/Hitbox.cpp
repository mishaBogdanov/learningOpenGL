#include "Hitbox.h"
#include "MyMath.h"
#include "World.h"
#include "Model.h"


Hitbox::Hitbox(std::vector<glm::vec3> given, std::vector<glm::vec3> gNormals, Model * model) {
	assignedModel = model;

	positionedVertices = given;
	vertices = given;
	normals = gNormals;
	rotatedNormals = gNormals;
}

void Hitbox::update(glm::mat4 * givenFull, glm::mat4* givenRotation) {
	transformation = givenFull;
	for (int i = 0; i < vertices.size(); i++) {
		positionedVertices[i] = (*transformation) * glm::vec4(vertices[i], 1);
	}
	rotation = givenRotation;
	for (int i = 0; i < normals.size(); i++) {
		rotatedNormals[i] = (*rotation) * glm::vec4(normals[i], 1);
	}

}

std::vector<glm::vec3> Hitbox::getNormalVectors() {
	return rotatedNormals;
}


int Hitbox::getNormalsSize() {
	return normals.size();
}


void Hitbox::getMaxMinFromProjection(glm::vec3& projectVec, float& max, float& min) {
	bool isFirst = true;
	for (int i = 0; i < positionedVertices.size(); i++) {
		glm::vec3 val;
		MyMath::projection(projectVec, positionedVertices[i], val);
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

glm::vec3* Hitbox::getVec(int i) {
	return &positionedVertices[i];
}

void Hitbox::addCollision(IntersectionModel given) {
	(*assignedModel).addIntersection(&given);
}

