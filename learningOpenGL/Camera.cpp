#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Camera::Camera(int gWidth, int gHeight, float gxScale, glm::vec3 gPosition) {
	position = gPosition;
	width = gWidth;
	height = gHeight;
	xScale = gxScale;
}

void Camera::setMatrix(float FOVdeg, float nearPlane, float farPlane, ShaderClass& shader, const char* uniform) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::perspective(glm::radians(FOVdeg), (float)width / height / xScale, nearPlane, farPlane);
	//projection = glm::lookAt()

}
