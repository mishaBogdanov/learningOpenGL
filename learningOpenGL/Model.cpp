#include "Model.h"
#pragma warning(disable:4996)
#include "Mesh.h"


Model::Model(std::string location) {
	load(location);
}

bool Model::load(std::string opening) {
	int curZero = 0;
	std::vector<glm::vec3> Vertices = {};
	std::vector<GLuint> Indices = {};
	FILE* file = fopen(opening.c_str(), "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;



		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			Vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			//if (matches != 9) {
			//	printf("File can't be read by our simple parser : ( Try exporting with other options\n");
			//	return false;
			//}
			Indices.push_back(vertexIndex[0]);
			Indices.push_back(vertexIndex[1]);
			Indices.push_back(vertexIndex[2]);
		}
		else if (strcmp(lineHeader, "o") == 0) {
			std::cout << "here";

			Mesh newMesh = Mesh(Vertices, Indices);

			mesh.push_back(newMesh);

			Vertices.clear();
			Indices.clear();
		}
	}
}


void Model::Draw(ShaderClass & shader, ShaderClass & shader2, Camera cam) {
	for (int i = 0; i < mesh.size(); i++) {
		mesh[i].Draw(shader, cam);
		mesh[i].Draw(shader2, cam);
	}
}
