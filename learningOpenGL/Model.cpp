#include "Model.h"
#pragma warning(disable:4996)
#include "Mesh.h"
#include <iostream>
#include <cstdio>


int tests(std::string given);

Model::Model(std::string location) {
	load(location);
	//tests("TeslaTruck.object");
}



void Model::Draw(ShaderClass & shader, ShaderClass & shader2, Camera cam) {
	for (int i = 0; i < mesh.size(); i++) {
		mesh[i].Draw(shader, cam);
		mesh[i].Draw(shader2, cam);
	}
}





bool Model::load(std::string given) {

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
				if (vertices[0] > Vertices.size()-1 || vertices[1] > Vertices.size()-1 || vertices[2] > Vertices.size()-1) {
					std::cout << "issue";
				}
				Indices.push_back(vertices[0]);
				Indices.push_back(vertices[1]);
				Indices.push_back(vertices[2]);
			}
			else if ((*fileIterator) == "o") {
				lastMaxVertexNumber = currentVertexesNumber+1;
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