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
	std::vector<Model* > modelsToCheck;
	GLFWwindow* window;

	const unsigned int SCR_WIDTH = 200;
	const unsigned int SCR_HEIGHT = 100;
	const double xScale = 1.8;
	bool isConsole = true;
	HANDLE hConsole;
	DWORD dwBytesWritten;
	wchar_t* screen = new wchar_t[200 * 100 + 1];
	float pixels[200 * 100 * 3] = { 0 };

	Camera cam = Camera(200, 100 * xScale, 1, glm::vec3(0.0f, 0.0f, 2.0f));

	void runUpdateCycle(std::vector<Model>* given);
	void updatePhysDeltaT();

	void comb(int N, int K, std::vector<int> & returning);
	bool colliding(Model & mod1, Model & model2);
	void generateAxis(Model& mod1, Model& model2, std::vector<glm::vec3>& returning);


public:
	World(float distX, float distY, float distZ, int divs);
	void setupGLFW();
	void update();
	void addFloor(int gNum, float gSpacing, float gHeight, float gz, float gx);

	void addModel(Model& given);
	void addModel(std::string given);
	void addModel(std::string given, float scale);

	void startRenderLoop();
	void screenToPixel();
	void clearScreen();
	void processInput();
	void renderModels();
	void renderFLoors(); //!!! ^
	void startUpdateCycle();

	
};

