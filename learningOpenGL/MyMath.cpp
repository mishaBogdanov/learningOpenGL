#include "MyMath.h"


void MyMath::vectorCross(glm::vec3& vec1, glm::vec3& vec2, glm::vec3& returning) {
	returning.x = vec1.y * vec2.z - vec1.z * vec2.y;
	returning.y = vec1.z * vec2.x - vec1.x * vec2.z;
	returning.z = vec1.x * vec2.y - vec1.y * vec2.x;
}

void MyMath::projection(glm::vec3& ontoProjection, glm::vec3& projected, glm::vec3& returning) {
	returning = ontoProjection * (ontoProjection.x * projected.x + ontoProjection.y * projected.y + ontoProjection.z * projected.z) /
		(ontoProjection.x * ontoProjection.x + ontoProjection.y * ontoProjection.y + ontoProjection.z * ontoProjection.z);
}