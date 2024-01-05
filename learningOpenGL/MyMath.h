#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class MyMath
{
public:
	static void projection(glm::vec3& ontoProjection, glm::vec3& projected, glm::vec3& returning);
	static void vectorCross(glm::vec3& vec1, glm::vec3& vec2, glm::vec3& returning);
};

