#include "Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>


Camera::Camera(int gWidth, int gHeight, float gxScale, glm::vec3 gPosition) {
	position = gPosition;
	width = gWidth;
	height = gHeight;
	xScale = gxScale;
	speed = 1;
	sensitivity = 50000;
	currentTime = glfwGetTime();
}

void Camera::setMatrix(float FOVdeg, float nearPlane, float farPlane, ShaderClass& shader, const char* uniform) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height / xScale, nearPlane, farPlane);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}


void Camera::Inputs(GLFWwindow* window) {
	double deltaT = glfwGetTime() - currentTime;
	currentTime = glfwGetTime();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += (float)(deltaT * speed) * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= (float)(deltaT * speed) * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= glm::normalize(glm::cross(Orientation, Up)) * (float)(deltaT * speed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += glm::normalize(glm::cross(Orientation, Up)) * (float)(deltaT * speed);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position += Up * (float)(deltaT * speed);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		position -= Up * (float)(deltaT * speed);
	}
	if (isInGame)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click


		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = (deltaT* sensitivity) * (float)(mouseY - (height / 2)) / height;
		float rotY = (deltaT * sensitivity) * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
}