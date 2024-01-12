#include "World.h"
#include <thread>
#include <iostream>
#include <windows.h>
#include "MyMath.h"
#include "Model.h"
#include "Hitbox.h"
#include <map>




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
	}
	else if (!isDriving) {
		cam.Inputs(window);
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !pushed) {
			isDriving = false;
			pushed = true;
		}
		cam.setView(window, (*(*drivable).getCm()) - (*drivable).getFacing() * 50.0f + glm::vec3(0, 20, 0), (*(*drivable).getCm()));
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
			if (appending.x != 0 || appending.y != 0 || appending.z != 0) {
				returning.push_back(appending);

			}
		}
	}
}




void World::detectCollisions() {
	std::vector<int> modelsToCheck;
	comb(models.size(), 2, modelsToCheck);
	for (int i = 0; i < modelsToCheck.size(); i += 2) {
		glm::vec3 normal;
		float intersectBy;
		IntersectionModel model;
		bool colliding = checkHitboxes(models[modelsToCheck[i]], models[modelsToCheck[i + 1]], model);
		if (colliding) {

			currentCollisions.push_back(model);
		}
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
		//detectPointFace(*models[0].getHitbox(0), *models[1].getHitbox(0));
		//detectPointFace(*models[1].getHitbox(0), *models[0].getHitbox(0));
		//detectEdgeEdge(*models[1].getHitbox(0), *models[0].getHitbox(0));

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



bool World::checkHitboxes(Model& model1, Model& model2, IntersectionModel& given) {
	float curDistance = 0;
	for (int i = 0; i < model1.getHitboxesSize(); i++) {
		for (int k = 0; k < model2.getHitboxesSize(); k++) {
			float intersectionDistance = 0;
			glm::vec3 normalCollision = glm::vec3(0);
			bool colliding = checkHitboxesColliding((*model1.getHitbox(i)), (*model2.getHitbox(k)), intersectionDistance, normalCollision);
			if (colliding && intersectionDistance > curDistance) {
				given.model1 = &model1;
				given.model2 = &model2;
				given.normal = normalCollision;
				given.amountIntersect = intersectionDistance;
				curDistance = intersectionDistance;
			}
		}
	}
	if (curDistance == 0) {
		return false;
	}
	else {
		return true;
	}
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
		}
		if (firstCheck) {
			if (mod1Max > mod2Min) {
				curintersect = mod1Max - mod2Min;
			}
			else {
				curintersect = mod2Max - mod1Min;
			}
			normalToIntersect = axis[i];
			firstCheck = false;
		}
		else if (mod1Max > mod2Min && curintersect > mod1Max - mod2Min) {
			curintersect = mod1Max - mod2Min;
			normalToIntersect = axis[i];
		}
		else if (mod1Min < mod2Max && curintersect > mod2Max - mod1Min) {
			curintersect = mod2Max - mod1Min;
			normalToIntersect = axis[i];
		}
	}

	return true;
}


void World::dealWithCollisions() {



	for (int i = 0; i < currentCollisions.size(); i++) {

		if (currentCollisions[i].model1->isMovable() && currentCollisions[i].model2->isMovable()) {
			dealWithBothMovable(i);
		}
		else if (currentCollisions[i].model1->isMovable()) {
			dealWithFirstMovable(i);
		}
		else if (currentCollisions[i].model2->isMovable()) {
			dealWithSecondMovable(i);
		}
	}
	currentCollisions.clear();
}

void World::dealWithBothMovable(int i) {


	glm::vec3 moveBy2 = currentCollisions[i].normal * currentCollisions[i].amountIntersect * 10.0f;
	glm::vec3 moveBy1 = moveBy2 * (-1.0f);


	std::vector<Contact> contacts;
	generateContacts(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	addImpulses(*currentCollisions[i].model1, *currentCollisions[i].model2, contacts);
	currentCollisions[i].model1->dealWithImpulses();
	currentCollisions[i].model2->dealWithImpulses();
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

void World::detectPointFace(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given) {
	std::vector<glm::vec3* > v1;
	std::map<glm::vec3*, float> valsFloat;
	std::map<glm::vec3*, glm::vec3> valsVector;
	for (int i = 0; i < h2.getVectorsSize(); i++) {
		v1.push_back(h2.getVec(i));
		valsFloat.insert({ h2.getVec(i) , -1 });
		valsVector.insert({ h2.getVec(i), glm::vec3(0) });
	}
	//!!!
	// !!! for now it's only checking against one box, have to make it against 2.
	for (int i = 0; i < h1.getNormalsSize(); i++) {
		glm::vec3 projectedCm = glm::vec3(0);
		MyMath::projection(*h1.getNormal(i), (*h1.getCm()), projectedCm);
		float cm = MyMath::getVecMultiple(*h1.getNormal(i), projectedCm);
		float max, min;
		h1.getMaxMin(i, cm, max, min);


		for (int k = 0; k < v1.size(); k++) {
			glm::vec3 v;
			MyMath::projection(*h1.getNormal(i), *v1[k], v);
			float multiple = MyMath::getVecMultiple(*h1.getNormal(i), v);
			if (multiple >= max || multiple <= min) {
				v1.erase(v1.begin() + k);
				k--;
			}
			else if (multiple < max && max - multiple > valsFloat[v1[k]]) {
				valsFloat[v1[k]] = max - multiple;
				valsVector[v1[k]] = *h1.getNormal(i);
			}
			else if (multiple > min && multiple - min > valsFloat[v1[k]]) {
				valsFloat[v1[k]] = multiple - min;
				valsVector[v1[k]] = *h1.getNormal(i);
			}
		}
	}
	for (int i = 0; i < v1.size(); i++) {
		Contact contact;
		contact.position = *v1[i];
		contact.normal = valsVector[v1[i]];
		given.push_back(contact);
	}
}

void World::correctNormalsToPointRightDirection(Model& h1, Model& h2, std::vector<Contact>& given) {
	for (int i = 0; i < given.size(); i++) {
		glm::vec3 cmToPos = given[i].position - *h1.getCm();
		if (glm::dot(cmToPos, given[i].normal) < 0) {
			given[i].normal = given[i].normal * (-1.0f);
		}
	}
}


glm::vec3 World::getClosestPointsOnLines(glm::vec3& a0, glm::vec3& b0, glm::vec3& a1, glm::vec3& b1, bool& worked) {
	glm::vec3 v1 = b0 - a0;
	glm::vec3 v2 = b1 - a1;
	glm::vec3 cross = glm::cross(v1, v2);
	if (MyMath::getVectorMagnitudeSquared(cross) < 0.001) {
		worked = false;
		return v1;
	}
	float t = glm::dot(glm::cross((a1 - a0), v2), cross) / MyMath::getVectorMagnitudeSquared(cross);
	worked = t < 0 ? false : t>1 ? false : true;
	return a0 + t * v1;
}

void World::detectEdgeEdge(Hitbox& h1, Hitbox& h2, std::vector<Contact>& given) {
	for (int i = 0; i < h1.getEdgeNumber(); i++) {
		for (int k = 0; k < h2.getEdgeNumber(); k++) {
			glm::vec3 a0, b0, a1, b1;
			h1.getEdge(i, a0, b0);
			h2.getEdge(k, a1, b1);
			bool worked1, worked2;
			glm::vec3 onh1 = getClosestPointsOnLines(a0, b0, a1, b1, worked1);
			glm::vec3 onh2 = getClosestPointsOnLines(a1, b1, a0, b0, worked2);

			glm::vec3 h1CmToOnH1 = *h1.getCm() - onh1;
			glm::vec3 h1CmToOnH2 = *h1.getCm() - onh2;

			glm::vec3 h2CmToOnH1 = *h2.getCm() - onh1;
			glm::vec3 h2CmToOnH2 = *h2.getCm() - onh2;

			glm::vec3 h1ToPointA = onh1 - a0;
			glm::vec3 h1ToPointB = onh1 - b0;
			glm::vec3 h2ToPointA = onh2 - a1;
			glm::vec3 h2ToPointB = onh2 - b1;

			glm::vec3 onh1ToOnH2 = onh2 - onh1;
			if (worked1 && worked2 &&
				MyMath::getVectorMagnitudeSquared(h1CmToOnH1) > MyMath::getVectorMagnitudeSquared(h1CmToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h2CmToOnH1) < MyMath::getVectorMagnitudeSquared(h2CmToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h1ToPointA) > MyMath::getVectorMagnitudeSquared(onh1ToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h1ToPointB) > MyMath::getVectorMagnitudeSquared(onh1ToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h2ToPointA) > MyMath::getVectorMagnitudeSquared(onh1ToOnH2) &&
				MyMath::getVectorMagnitudeSquared(h2ToPointB) > MyMath::getVectorMagnitudeSquared(onh1ToOnH2)
				) {
				glm::vec3 appending = (onh1 + onh2) / 2.0f;
				//std::cout << "\n\n this Ran " << onh1.x << " " << onh1.y << " " << onh1.z << " on vertices: \n =====================\n" 
				//	<< a0.x << " " << a0.y << " " << a0.z << "\n" << b0.x << " " << b0.y << " " << b0.z << "\n\n" 
				//	<< a1.x << " " << a1.y << " " << a1.z << "\n" << b1.x << " " << b1.y << " " << b1.z <<  " "<< "\n" << i << " " << k << "\n======================\n";
				//std::cout << "this Ran  " << onh2.x << " " << onh2.y << " " << onh2.z << "\n";
				//std::cout << i << " " << k << "\n";




				Contact contact;
				contact.position = appending;
				glm::vec3 normal = glm::normalize(glm::cross(b0 - a0, b1 - a1));
				if (glm::dot(appending - *h1.getCm(), normal) < 0) {
					normal = normal * (-1.0f);
				}
				contact.normal = normal;
				given.push_back(contact);
			}
		}
	}
}

void World::generateContacts(Model& model1, Model& model2, std::vector <Contact>& given) {
	for (int i = 0; i < model1.getHitboxesSize(); i++) {
		for (int k = 0; k < model2.getHitboxesSize(); k++) {
			detectPointFace(*model1.getHitbox(i), *model2.getHitbox(k), given);
			detectPointFace(*model2.getHitbox(i), *model1.getHitbox(k), given);
			correctNormalsToPointRightDirection(model1, model2, given);

			//std::cout << given.size() << "\n";
			//for (int i = 0; i < given.size(); i++) {
			//	std::cout << "\nvec: \n";
			//	std::cout << given[i].position.x << " " << given[i].position.y << " " << given[i].position.z << "\n";
			//	std::cout << given[i].normal.x << " " << given[i].normal.y << " " << given[i].normal.z << "\n";
			//}
			detectEdgeEdge(*model1.getHitbox(i), *model2.getHitbox(k), given);
			//std::cout << given.size() << "\n";
			//for (int i = 0; i < given.size(); i++) {
			//	std::cout << "\nvec: \n";
			//	std::cout << given[i].position.x << " " << given[i].position.y << " " << given[i].position.z << "\n";
			//	std::cout << given[i].normal.x << " " << given[i].normal.y << " " << given[i].normal.z << "\n";
			//}

		}
	}
}


void World::addImpulses(Model& model1, Model& model2, std::vector<Contact>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		//glm::vec3 cm1ToCollision = *model1.getCm();
		//glm::vec3 cm2ToCollision = *model2.getCm();
		float bounceFactor = model1.getBounceFactor() * model2.getBounceFactor();
		float frictionFactor = model1.getFrictionFactor() * model2.getFrictionFactor();

		float scale = (glm::dot(model1.getVelocity() - model2.getVelocity(), vec[i].normal) + glm::dot(*model1.getAngularVelocity() - *model2.getAngularVelocity(), vec[i].normal)) * bounceFactor * 1.0f;

		Impulse impOn2;
		impOn2.direction = vec[i].normal * scale / (float)vec.size();
		impOn2.position = vec[i].position;

		Impulse impOn1;
		impOn1.direction = impOn2.direction * (-1.0f);
		impOn1.position = vec[i].position;

		model1.addImpulse(impOn1);
		model2.addImpulse(impOn2);
	}
}




