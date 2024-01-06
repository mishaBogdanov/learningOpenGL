#include "World.h"
#include <thread>
#include <iostream>
#include <windows.h>
#include "MyMath.h"





void framebuffer_size_callback(GLFWwindow* window, int width, int height);

World::World(float distX, float distY, float distZ, int divs) {
	sizeX = distX;
	sizeY = distY;
	sizeZ = distZ;
	divisions = divs;
}

void World::setupGLFW() {
	// glfw: initialize and configure
		// ------------------------------
		glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	//GLFW.glfwWindowHint(GLFW.GLFW_VISIBLE, GLFW.GLFW_FALSE);
	//if (isConsole) {
	//	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	//}
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);



	time = glfwGetTime();
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	dwBytesWritten = 0;
	SetConsoleActiveScreenBuffer(hConsole);

}

void World::addFloor(int gNum, float gSpacing, float gHeight, float gz, float gx) {
	floors.push_back(Floor(gNum, gSpacing, gHeight, gz, gx));

}

void World::addModel(std::string given) {
	models.push_back(Model(given));
}

void World::addModel(std::string given, float scale) {
	models.push_back(Model(given, scale));
}


void World::addModel(std::string given, float scale, glm::vec3 pos) {
	models.push_back(Model(given, scale, pos));
}

void World::setVelocity(int location, glm::vec3 v) {
	models[location].setVelocity(v);
}


void World::rotateModel(int location, float angle, glm::vec3& norm) {
	models[location].rotate(angle, norm);
}

void World::screenToPixel() {

	if (isConsole) {

		glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_FLOAT, pixels);

		for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT * 3; i += 3) {

			int pos = 0;
			if (i / 3 / SCR_WIDTH == 0) {
				pos = (SCR_HEIGHT - 1) * SCR_WIDTH + i / 3;
				//std::cout << i / 3;
			}
			else {
				pos = (SCR_HEIGHT - (i / 3 / SCR_WIDTH) - 1) * SCR_WIDTH + ((i / 3) % SCR_WIDTH);
				//std::cout << pos;
			}
			

			float colorValue = pixels[i] + pixels[i + 1] + pixels[i + 2];

			//std::cout << colorValue << std::endl;

			if (colorValue < 0.1) {
				screen[pos] = ' ';
			}
			else if (colorValue < 0.6) {
				screen[pos] = ',';
			}
			else if (colorValue < 1.2) {
				screen[pos] = ';';
			}
			else if (colorValue < 1.8) {
				screen[pos] = 'i';
			}
			else if (colorValue < 2.2) {
				screen[pos] = '0';
			}
			else if (colorValue < 2.6) {
				screen[pos] = 0x2592;
			}
			else if (colorValue < 8) {
				screen[pos] = 0x2593;
			}
			else {
				screen[pos] = 0x2588;
			}


		}
		screen[200 * 100] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, 200 * 100, { 0,0 }, &dwBytesWritten);
	}
}

void World::clearScreen() {
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void World::processInput() {
	cam.Inputs(window);
}

void World::renderModels() {
	for (int i = 0; i < models.size(); i++) {
		models[i].Draw(cam);
	}

	for (int i = 0; i < floors.size(); i++) {
		floors[i].Draw(cam);
	}

}

void World::update() {
	for (int i = 0; i < models.size(); i++) {
		models[i].update(deltaT);
	}
}



void World::updatePhysDeltaT() {
	physDeltaT = physT - glfwGetTime();
	physT = glfwGetTime();
}





void World::runUpdateCycle(std::vector<Model>* given) {
	physT = glfwGetTime();
	while (true) {
		//test(given, physDeltaT);
		double time = glfwGetTime();
		Sleep((physT + 1/2 - time) * 1000);
		updatePhysDeltaT();
	}
}


void World::comb(int N, int K, std::vector<int> & returning) {
	std::string bitmask(K, 1); // K leading 1's
	bitmask.resize(N, 0); // N-K trailing 0's

	// print integers and permute bitmask
	do {
		for (int i = 0; i < N; ++i) // [0..N-1] integers
		{
			if (bitmask[i]) returning.push_back(i);
		}
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}



bool World::colliding(Model& mod1, Model& mod2, float & curintersect, glm::vec3 & normalToIntersect) {
	std::vector<glm::vec3> axis;
	generateAxis(mod1, mod2, axis);


	for (int i = 0; i < axis.size(); i++) {
		float mod1Max, mod1Min, mod2Max, mod2Min;
		mod1.getMaxMinFromProjection(axis[i], mod1Max, mod1Min);
		mod2.getMaxMinFromProjection(axis[i], mod2Max, mod2Min);
		if (mod1Max < mod2Min || mod1Min > mod2Max) {
			return false;
		}
		else if (mod1Max > mod2Min && curintersect < mod1Max - mod2Min) {
			curintersect = mod1Max - mod2Min;
			normalToIntersect = axis[i];
		}
		else if (mod1Min < mod2Max && curintersect < mod2Max - mod1Min) {
			curintersect = mod2Max - mod1Min;
			normalToIntersect = axis[i];
		}
	}

	return true;
}

void World::generateAxis(Model& mod1, Model& mod2, std::vector<glm::vec3>& returning) {
	std::vector<glm::vec3> l1 = mod1.getHitboxVectors();
	std::vector<glm::vec3> l2 = mod2.getHitboxVectors();
	for (int i = 0; i < l1.size(); i++) {
		returning.push_back(l1[i]);
	}

	for (int i = 0; i < l2.size(); i++) {
		returning.push_back(l2[i]);
	}

	for (int i = 0; i < l1.size(); i++) {
		for (int k = 0; k < l2.size(); k++) {
			glm::vec3 appending;
			MyMath::vectorCross(l1[i], l2[k], appending);
			returning.push_back(appending);
		}
	}
}


void World::projectModel(Model& given, glm::vec3& givenVector, float& max, float& min) {
	
}

void World::detectCollisions() {
	std::vector<int> modelsToCheck;
	comb(models.size(), 2, modelsToCheck);
	for (int i = 0; i < modelsToCheck.size(); i += 2) {
		glm::vec3 normal;
		float intersectBy;
		if (colliding(models[modelsToCheck[i]], models[modelsToCheck[i + 1]], intersectBy, normal)) {
			IntersectionModel model;
			model.model1 = &models[modelsToCheck[i]];
			model.model2 = &models[modelsToCheck[i + 1]];
			model.amountIntersect = intersectBy;
			model.normal = normal;
			currentCollisions.push_back(model);
		}
	}
}

void World::dealWithCollisions() {

	for (int i = 0; i < currentCollisions.size(); i++) {
		glm::vec3 t = currentCollisions[i].normal * currentCollisions[i].amountIntersect/100.0f;
		(*currentCollisions[i].model2).moveBy(t);
	}
	currentCollisions.clear();
}

















void World::startRenderLoop() {

	while (!glfwWindowShouldClose(window))
	{
		deltaT = glfwGetTime() - time;
		time = glfwGetTime();
		// input
		// -----

		processInput();


		// render
		// ------

		clearScreen();

		// be sure to activate the shader before any calls to glUniform
		//glBindTexture(GL_TEXTURE_2D, texture);


		cam.setMatrix(90.0f, 0.1f, 300.0f);
		update();
		detectCollisions();
		dealWithCollisions();

		renderModels();

		screenToPixel();




		processInput();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}