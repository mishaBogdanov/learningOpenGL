#include "Model.h"
#pragma warning(disable:4996)
#include "Mesh.h"
#include <iostream>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>





Model::Model(std::string location) {
	load(location, 1);
	setupModel();


}

void Model::setupModel() {
	translation = glm::mat4(1);
	velocity = glm::vec3(0, 0, 0);
	angularVelocityDirection = glm::vec3(1, 0, 0);
	angularVelocity = 0;
	ShaderClass shaderProgram("default.vert", "default.geom", "default.frag");
	ShaderClass shaderProgram2("default.vert", "outline.geom", "outline.frag");
	shaders.push_back(shaderProgram);
	shaders.push_back(shaderProgram2);
}

Model::Model(std::string location, float scale) {
	load(location, scale);
	setupModel();


}





void Model::Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam) {
	for (int i = 0; i < mesh.size(); i++) {
		mesh[i].Draw(shader, cam);
		mesh[i].Draw(shader2, cam);
	}
}

glm::mat4 Model::getTransformation() {
	return glm::translate(glm::mat4(1), cm) * translation;
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

	translation = glm::rotate(translation, 0.01f, glm::vec3(0, 1, 1));


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




bool Model::load(std::string given, float scale) {

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
		corners[0] = glm::vec3(PosX, PosY, PosZ);
		corners[1] = glm::vec3(PosX, PosY, NegZ);
		corners[2] = glm::vec3(PosX, NegY, PosZ);
		corners[3] = glm::vec3(PosX, NegY, NegZ);
		corners[4] = glm::vec3(NegX, PosY, PosZ);
		corners[5] = glm::vec3(NegX, PosY, NegZ);
		corners[6] = glm::vec3(NegX, NegY, PosZ);
		corners[7] = glm::vec3(NegX, NegY, NegZ);

		glm::vec3 currentSum = corners[0];
		for (int i = 1; i < 8; i++) {
			currentSum += corners[i];
		}
		cm = currentSum / 8.0f;

		file.close();
	}

	return 0;
}

