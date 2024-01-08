#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
//#include "Model.h"
//#include "World.h"

//class World;
class Model;
struct IntersectionModel;
//void Model::addIntersection(IntersectionModel* given);


class Hitbox
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> positionedVertices;

	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> rotatedNormals;

	glm::mat4* transformation;
	glm::mat4* rotation;
	Model* assignedModel;
	int type; //for future implementation in case I need wheels, body, etc.
public:
	Hitbox(std::vector<glm::vec3> given, std::vector<glm::vec3> gNormals, Model * model);
	void update(glm::mat4 *givenMatrix, glm::mat4 * givenRotation);
	std::vector<glm::vec3> getNormalVectors();
	int getNormalsSize();
	void getMaxMinFromProjection(glm::vec3& projectVec, float& max, float& min);
	glm::vec3* getVec(int i);
	void addCollision(IntersectionModel model);

	
};

