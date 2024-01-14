#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "ExperimentingWithShaders.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "MyMath.h"

#include "World.h"
#include <chrono>
#include <thread>




int main()
{
	using std::operator""s;
	//ExperimentingWithShaders t = ExperimentingWithShaders();
	//std::this_thread::sleep_for(6s);

	World test = World(0, 0, 0, 0);
	test.setupGLFW();
	test.addFloor(10, 100, 0, -30, 0);
	test.addModel("BasicCube.object", 9, glm::vec3(0, 9+30, 0));
	test.addModel("BasicCube.object", 3, glm::vec3(50, 2 + 30,9));
	test.addModel("BasicCube.object", 15, glm::vec3(-100, 2 + 30, 9));
	test.addModel("TeslaTruck.object", 20, glm::vec3(0, 2 + 30, -200));




	//test.addModel("TeslaTruck.object", 10, glm::vec3(0,12,30));
	//test.setVelocity(0, glm::vec3(0, 0, 40));
	test.setVelocity(1, glm::vec3(-30, 0, 0));
	test.setVelocity(2, glm::vec3(30, 0, 0));
	test.setVelocity(3, glm::vec3(0, 0, 30));


	//test.changeIsMovable(0, false);
	glm::vec3 t = glm::vec3(0, 1, 0);
	glm::vec3 t2 = glm::vec3(-1, 0, 0);
	test.rotateModel(1, 45, t);
	test.rotateModel(1, -45, t2);
	test.rotateModel(0, 8, t2);
	//test.rotateModel(0, 60.0f, t2);
	test.startRenderLoop();



	return 0;


}





