#include "World.h"
#include <thread>
#include <iostream>
#include <windows.h>
#include "MyMath.h"
#include "Model.h"
#include "Hitbox.h"





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
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !isDriving && !pushed) {
		isDriving = true;
		pushed = true;
		drivable = &models[0];
	}
	else if ((glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS && pushed)) {
		pushed = false;
	}else if (!isDriving) {
		cam.Inputs(window);
	}else{
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !pushed) {
			isDriving = false;
			pushed = true;
		}
	cam.setView(window, (*(*drivable).getCm()) - (*drivable).getFacing() * 50.0f + glm::vec3(0,20,0), (*(*drivable).getCm()));
	}
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










void World::comb(int N, int K, std::vector<int>& returning) {
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






void World::changeIsMovable(int pos, bool newValue) {
	models[pos].changeIsMovable(newValue);
}



void World::generateAxis_Hitboxes(Hitbox& hitbox1, Hitbox& hitbox2, std::vector<glm::vec3>& returning) {
	std::vector<glm::vec3> l1 = hitbox1.getNormalVectors();
	std::vector<glm::vec3> l2 = hitbox2.getNormalVectors();
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




void World::detectCollisions() {
	std::vector<int> modelsToCheck;
	comb(models.size(), 2, modelsToCheck);
	for (int i = 0; i < modelsToCheck.size(); i += 2) {
		glm::vec3 normal;
		float intersectBy;
		checkHitboxes(models[modelsToCheck[i]], models[modelsToCheck[i + 1]]);
	}
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




		//processInput();
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



bool World::checkHitboxes(Model& model1, Model& model2) {

	for (int i = 0; i < model1.getHitboxesSize(); i++) {
		for (int k = 0; k < model2.getHitboxesSize(); k++) {
			float intersectionDistance = 0;
			glm::vec3 normalCollision = glm::vec3(0);
			bool colliding = checkHitboxesColliding((*model1.getHitbox(i)), (*model2.getHitbox(k)), intersectionDistance, normalCollision);
			if (colliding) {
				//glfwSetWindowShouldClose(window, true);
				//return true; //not right
				IntersectionModel model;
				model.model1 = &model1;
				model.model2 = &model2;
				model.normal = normalCollision;
				model.amountIntersect = intersectionDistance;
				currentCollisions.push_back(model);
			}
		}
	}
	return false; //not right
}


bool World::checkHitboxesColliding(Hitbox& hitbox1, Hitbox& hitbox2, float& curintersect, glm::vec3& normalToIntersect) {
	std::vector<glm::vec3> axis;
	generateAxis_Hitboxes(hitbox1, hitbox2, axis);
	bool firstCheck = true;

	for (int i = 0; i < axis.size(); i++) {
		float mod1Max, mod1Min, mod2Max, mod2Min;
		hitbox1.getMaxMinFromProjection(axis[i], mod1Max, mod1Min);
		hitbox2.getMaxMinFromProjection(axis[i], mod2Max, mod2Min);
		if (mod1Max < mod2Min || mod1Min > mod2Max) {
			return false;
		}if (firstCheck) {
			if (mod1Max > mod2Min) {
				curintersect = mod1Max - mod2Min;
			}
			else {
				curintersect = mod2Max - mod1Min;
			}
			normalToIntersect = axis[i];
			firstCheck = false;
		}
		else if (mod1Max > mod2Min && abs(curintersect) > abs(mod1Max - mod2Min)) {
			curintersect = mod1Max - mod2Min;
			normalToIntersect = axis[i];
		}
		else if (mod1Min < mod2Max && abs(curintersect) > abs(mod2Max - mod1Min)) {
			curintersect = mod1Min - mod2Max;
			normalToIntersect = axis[i];
		}
	}

	return true;
}


void World::dealWithCollisions() {
	//if (currentCollisions.size() != 0) {
	//	std::cout << currentCollisions[0].normal.x << " " << currentCollisions[0].normal.y << " " << currentCollisions[0].normal.z << "\n";
	//	glfwSetWindowShouldClose(window, true);
	//}


	for (int i = 0; i < currentCollisions.size(); i++) {
		if (currentCollisions[i].model1->isMovable() && currentCollisions[i].model2->isMovable()) {
			dealWithBothMovable(i);
		}
		else if(currentCollisions[i].model1->isMovable()){
			dealWithFirstMovable(i);
		}
		else if (currentCollisions[i].model2->isMovable()) {
			dealWithSecondMovable(i);
		}
	}
	currentCollisions.clear();
}

void World::dealWithBothMovable(int i) {
	glm::vec3 moveBy2 = currentCollisions[i].normal * currentCollisions[i].amountIntersect / 2.0f;
	glm::vec3 moveBy1 = moveBy2 * (-1.0f);

	currentCollisions[i].model1->moveBy(moveBy1);
	currentCollisions[i].model2->moveBy(moveBy2);

}

void World::dealWithFirstMovable(int i) {
	glm::vec3 moveBy = currentCollisions[i].normal * (-1.0f) * currentCollisions[i].amountIntersect;
	currentCollisions[i].model1->moveBy(moveBy);
}

void World::dealWithSecondMovable(int i) {
	glm::vec3 moveBy = currentCollisions[i].normal * currentCollisions[i].amountIntersect;
	currentCollisions[i].model2->moveBy(moveBy);
}
