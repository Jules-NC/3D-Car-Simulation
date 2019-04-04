#ifndef CONTROLS_HPP
#define CONTROLS_HPP

const int WIDTH = 1024;
const int HEIGHT = 768;

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif