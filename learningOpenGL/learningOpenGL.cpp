#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "ExperimentingWithShaders.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "World.h"


void comb(int N, int K, std::vector<int>& returning);

int main()
{
    //ExperimentingWithShaders t = ExperimentingWithShaders();

    World test = World(0, 0, 0, 0);
    test.setupGLFW();
    test.addFloor(10, 100, 0, 0, 0);
    test.addModel("TeslaTruck.object", 10);
    test.startUpdateCycle();
    test.startRenderLoop();




    return 0;
}

