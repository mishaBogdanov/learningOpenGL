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


void comb(int N, int K, std::vector<int>& returning);

int main()
{
    //ExperimentingWithShaders t = ExperimentingWithShaders();

    World test = World(0, 0, 0, 0);
    test.setupGLFW();
    test.addFloor(10, 100, 0, 0, 0);
    test.addModel("TeslaTruck.object", 10, glm::vec3(0, 9.9, -200));
    test.addModel("TeslaTruck.object", 10, glm::vec3(0,12,200));
    test.setVelocity(0, glm::vec3(0, 0, 40));
    test.setVelocity(1, glm::vec3(0, 0, -80));
    glm::vec3 t = glm::vec3(0, 1, 1);
    glm::vec3 t2 = glm::vec3(1, 1, 1);
    test.rotateModel(1, 30, t);
    test.rotateModel(0, 60.0f, t2);
    test.startRenderLoop();





    return 0;
}

