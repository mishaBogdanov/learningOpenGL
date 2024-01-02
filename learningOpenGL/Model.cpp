#include "Model.h"
#pragma warning(disable:4996)
#include "Mesh.h"
#include <iostream>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>





Model::Model(std::string location) {
	load(location, 1);
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
	translation = glm::mat4(1);
	velocity = glm::vec3(0, 0, 0);
	angularVelocityDirection = glm::vec3(1, 0, 0);
	angularVelocity = 0;
	ShaderClass shaderProgram("default.vert", "default.geom", "default.frag");
	ShaderClass shaderProgram2("default.vert", "outline.geom", "outline.frag");
	shaders.push_back(shaderProgram);
	shaders.push_back(shaderProgram2);


}



void Model::Draw(ShaderClass& shader, ShaderClass& shader2, Camera cam) {
	for (int i = 0; i < mesh.size(); i++) {
		mesh[i].Draw(shader, cam);
		mesh[i].Draw(shader2, cam);
	}
}


void Model::Draw(Camera cam) {
	for (int i = 0; i < mesh.size(); i++) {
		for (int k = 0; k < shaders.size(); k++) {
			mesh[i].Draw(shaders[k], cam);
		}

	}
}

void Model::update(float deltaT) {
	translation = glm::translate(translation, velocity * deltaT);
	translation = glm::rotate(translation, static_cast<float>(glm::radians(angularVelocity * deltaT)), angularVelocityDirection);
	for (int k = 0; k < shaders.size(); k++) {
		shaders[k].Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaders[k].ID, "positionMatrix"), 1, GL_FALSE, glm::value_ptr(translation));
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

	if (file.is_open()) {
		std::istream_iterator<std::string> fileIterator(file);
		std::istream_iterator<std::string> endIterator;

		while (fileIterator != endIterator) {
			if ((*fileIterator) == "v") {
				currentVertexesNumber += 1;
				++fileIterator;
				float val1 = std::stof((*fileIterator));
				++fileIterator;
				float val2 = std::stof((*fileIterator));
				++fileIterator;
				float val3 = std::stof((*fileIterator));
				//++fileIterator;


				Vertices.push_back(glm::vec3(val1, val2, val3) * scale);
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
				if (vertices[0] > Vertices.size() - 1 || vertices[1] > Vertices.size() - 1 || vertices[2] > Vertices.size() - 1) {
					std::cout << "issue";
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

		file.close();
	}

	return 0;
}

