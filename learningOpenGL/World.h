#pragma once
#include "Model.h"
#include <vector>
#include "Camera.h"

class World
{
private:
	float size;
	int divisions;
	std::vector<Model> models;
	Camera cam;
public:
	World(float distX);
	
};

