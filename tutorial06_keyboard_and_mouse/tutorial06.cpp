// STANDARDS
#include <stdio.h>						// |
#include <stdlib.h>						// |
#include <vector>						// |
#include <iostream>
										// LIBS
#include <GL/glew.h>					// |glew
#include <glfw3.h>						// |GLFW
#include <glm/glm.hpp>					// |glm
#include <common/shader.hpp>			// |
#include <common/controls.hpp>			// |
										// LIBS
#include "Test.h"
#include "GUI.h"
#include "Physics.h"
#include "SControls.h"
using namespace glm;				// NAMESPACE
unsigned int WIDTH = 1024;
unsigned int HEIGHT = 768;
GLFWwindow * WINDOW;
float SPEED;

bool initGLEW();

glm::mat4 CarCamera = glm::mat4();

float	gEngineForce = 0.f;
float	gBreakingForce = 0.f;
float	maxEngineForce = 60.f;//this should be engine/velocity dependent
float	gVehicleSteering = 0.f;
float	steeringIncrement = 0.04f;
float	steeringClamp = 0.5f;


bool initGLEW() {
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	return true;
}


int main(void)
{
	SControls::getInstance().init(WIDTH, HEIGHT);
	WINDOW = SControls::getInstance().getWindows();

	initGLEW();

	// BULLET
	Physics::getInstance().initPhysics();
	btDiscreteDynamicsWorld* dynamicsWorld = Physics::getInstance().getDynamicsWorld();
	int ground = Physics::getInstance().createGround(btVector3(50, 3, 50));
	int vehicleID = Physics::getInstance().createCar(gEngineForce, gBreakingForce, gVehicleSteering);
	btRaycastVehicle * vehicle = Physics::getInstance().getVehicle(vehicleID);

	// GUI
	GUI::getInstance().init("Barre");
	float  lol = 156;
	GUI::getInstance().addVar(TW_TYPE_FLOAT, &lol);

	// SHADERS
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");


	// ======================== DESSIN ============================
	char * path = "C:/Users/jules/Desktop/eaze/AAA/bobross.jpg";
	char * path2 = "C:/Users/jules/Desktop/eaze/AAA/blandere.jpg";
	char * objSuzy = "suzanne.obj";
	char * objSquare = "C:/Users/jules/Desktop/eaze/AAA/square.obj";
	char * objWheel = "C:/Users/jules/Desktop/eaze/AAA/wheel.obj";

	Test square = Test();
	Test wheel = Test();
	Test cGround = Test();

	square.init(objSquare, path);
	wheel.init(objWheel, path2);
	cGround.init("C:/Users/jules/Desktop/eaze/AAA/ground.obj", path);

	int box1 = Physics::getInstance().createBox(1, btVector3(1, 3, 1), btVector3(1, 1, 1));
	Physics::getInstance().createBox(0.1, btVector3(3, 3, 3), btVector3(15, 1, 3));
	Physics::getInstance().createSphere(8, 1, btVector3(0, 1, 10));
	// ======================== /DESSIN ===========================


	BulletDebugDrawer_DeprecatedOpenGL mydebugdrawer;
	dynamicsWorld->setDebugDrawer(&mydebugdrawer);
	// BOB ROSS CUBIC SPHERE

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		double currentTime = glfwGetTime();

		nbFrames++;
		float deltaTime = currentTime - lastTime;

		if (currentTime - lastTime >= 1.0) {
			printf("%f ms/frame\n",1000- 1000.0 / double(deltaTime));
			nbFrames = 0;
			lastTime += 1.0;
		}
		gBreakingForce = 0.1f;
		dynamicsWorld->stepSimulation(deltaTime, 5);

		Physics::getInstance().applyForcesOnCar(vehicleID, gEngineForce, gBreakingForce, gVehicleSteering);
		glm::mat4 chassisMatrix = Physics::getInstance().getChassisMatrix(vehicleID);

		CarCamera = glm::translate(chassisMatrix, glm::vec3(0, +4, 0));
		CarCamera = glm::translate(CarCamera, glm::vec3(0, 0, -11));
		CarCamera = glm::rotate(CarCamera, 180.0f, glm::vec3(0, 1, 0));

		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 MVP;

		// Roues
		std::vector<glm::mat4> wheelsMatrix = Physics::getInstance().getWheelsMatrix(vehicleID);
		for (int i = 0; i < wheelsMatrix.size(); i++) {
			MVP = ProjectionMatrix * ViewMatrix* wheelsMatrix[i];
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			wheel.draw();
		}

		glm::mat4 model = Physics::getInstance().getModelMatrix(ground);

		MVP = ProjectionMatrix * ViewMatrix * model;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		cGround.draw();

		MVP = ProjectionMatrix * ViewMatrix* chassisMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		square.draw();

		model = Physics::getInstance().getModelMatrix(box1);
		MVP = ProjectionMatrix * ViewMatrix* model;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		square.draw();

		mydebugdrawer.SetMatrices(ViewMatrix, ProjectionMatrix);
		dynamicsWorld->debugDrawWorld();
		
		GUI::getInstance().drawBar();

		// Swap buffers
		glfwSwapBuffers(WINDOW);
		glfwPollEvents();
	} 
	while (glfwGetKey(WINDOW, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(WINDOW) == 0);

	square.FreeMemory();
	glDeleteProgram(programID);
	glfwTerminate();

	return 0;
}
