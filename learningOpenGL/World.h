#pragma once
//#include "ExperimentingWithShaders.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"
#include <stb/stb_image.h>
#include "TextureClass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <windows.h>
#include "Camera.h"
#include <vector>
#include "Mesh.h"
#include "Model.h"
#include "floor.h"
#include <algorithm>


struct IntersectionModel {
	Model* model1;
	Model *model2;
	float amountIntersect;
	glm::vec3 normal;
} ;


class World
{
private:
	float sizeX;
	float sizeY;
	float sizeZ;
	int divisions;
	

	double physT;
	double physDeltaT;
	double deltaT;
	float time;
	std::vector<Model> models;
	std::vector<Floor> floors;
	GLFWwindow* window;

	std::vector<IntersectionModel> currentCollisions;

	const unsigned int SCR_WIDTH = 200;
	const unsigned int SCR_HEIGHT = 100;
	const double xScale = 1.8;
	bool isConsole = true;
	HANDLE hConsole;
	DWORD dwBytesWritten;
	wchar_t* screen = new wchar_t[200 * 100 + 1];
	float pixels[200 * 100 * 3] = { 0 };

	Camera cam = Camera(200, 100 * xScale, 1, glm::vec3(0.0f, 0.0f, 2.0f));

	bool pushed = false;
	bool isDriving = false;
	Model* drivable;


	void runUpdateCycle(std::vector<Model>* given);
	void updatePhysDeltaT();

	void comb(int N, int K, std::vector<int> & returning);
	bool colliding(Model& mod1, Model& mod2, float& curintersect, glm::vec3& normalToIntersect);
	void generateAxis(Model& mod1, Model& model2, std::vector<glm::vec3>& returning);
	void projectModel(Model& given, glm::vec3& givenVector, float& max, float& min);
	void detectCollisions();
	void dealWithCollisions();
	void dealWithBothMovable(int i);


public:
	World(float distX, float distY, float distZ, int divs);
	void setupGLFW();
	void update();
	void addFloor(int gNum, float gSpacing, float gHeight, float gz, float gx);

	void addModel(Model& given);
	void addModel(std::string given);
	void addModel(std::string given, glm::vec3 location);
	void addModel(std::string given, float scale, glm::vec3 location);
	void addModel(std::string given, float scale);
	void addModel(std::string given, float scale, bool v, glm::vec3 velocity);

	void rotateModel(int location, float angle, glm::vec3& norm);
	void setVelocity(int index, glm::vec3 newVelocity);

	void startRenderLoop();
	void screenToPixel();
	void clearScreen();
	void processInput();
	void renderModels();
	void renderFLoors(); //!!! ^

	
};

