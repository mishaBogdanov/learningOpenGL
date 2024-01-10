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



void getClosestPoints(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4) {
    glm::vec3 a = glm::normalize(v2 - v1);
    glm::vec3 b = glm::normalize(v4 - v3);
    glm::vec3 cn = glm::normalize(glm::cross(b, a));
    glm::vec3 projection_ = glm::dot(v3 - v1, a) * a;
    glm::vec3 rejection = v3 - v1 - glm::dot(v3 - v1, a) * a - glm::dot(v3 - v1, cn) * cn;
    glm::vec3 closest_approach = v3 - b * glm::normalize(rejection) / glm::dot(b, glm::normalize(rejection));
    std::cout << (v1 + projection_).x << " " << (v1 + projection_).y << " " << (v1 + projection_).z << "\n";
    std::cout << (closest_approach).x << " " << closest_approach.y << " " << closest_approach.z << "\n";

}

int main()
{
    //ExperimentingWithShaders t = ExperimentingWithShaders();

    World test = World(0, 0, 0, 0);
    test.setupGLFW();
    test.addFloor(10, 100, 0, 0, 0);
    test.addModel("BasicCube.object", 9, glm::vec3(0, 12, 0));
    test.addModel("BasicCube.object", 3, glm::vec3(0, 40, 0));

    //test.addModel("TeslaTruck.object", 10, glm::vec3(0,12,30));
    //test.setVelocity(0, glm::vec3(0, 0, 40));
    test.setVelocity(1, glm::vec3(0, -3, 0));
    test.changeIsMovable(0, false);
    //glm::vec3 t = glm::vec3(0, 1, 0);
    //glm::vec3 t2 = glm::vec3(1, 1, 1);
    //test.rotateModel(1, 30, t);
    //test.rotateModel(0, 60.0f, t2);
    //test.startRenderLoop();
    glm::vec3 t1 = glm::vec3(1, 2, 3);
    glm::vec3 t2 = glm::vec3(4, 5, 6);
    glm::vec3 t3 = glm::vec3(10, 8, 9);
    glm::vec3 t4 = glm::vec3(12, 15, 12);
    getClosestPoints(t1, t2, t3, t4);
    float t = glm::dot((glm::cross((t3 - t2), glm::vec3(2, 7, 3))), glm::cross(glm::vec3(3, 3, 3), glm::vec3(2, 7, 3))) / glm::length(glm::cross(glm::vec3(3, 3, 3), glm::vec3(2, 7, 3))) / glm::length(glm::cross(glm::vec3(3, 3, 3), glm::vec3(2, 7, 3)));
    glm::vec3 t5 = t1 + t * glm::vec3(3, 3, 3);
    std::cout << (t5).x << " " << t5.y << " " << t5.z << "\n";
    return 0;


}



