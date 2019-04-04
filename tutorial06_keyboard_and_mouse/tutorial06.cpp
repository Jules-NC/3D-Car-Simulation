// STANDARDS
#include <stdio.h>						// |
#include <stdlib.h>						// |
#include <vector>						// |
#include <iostream>
										// LIBS
#include <GL/glew.h>					// |glew
#include <glfw3.h>						// |GLFW
#include <glm/glm.hpp>					// |glm
#include <glm/gtc/matrix_transform.hpp>	// ||
#include <glm/gtc/quaternion.hpp>		// ||
#include <glm/gtx/quaternion.hpp>		// ||
										// COMMONS
#include <common/shader.hpp>			// |
#include <common/texture.hpp>			// |
#include <common/controls.hpp>			// |
										// LIBS
#include <btBulletDynamicsCommon.h>		// |Bullet
#include <assimp/Importer.hpp>			// |Assimp
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <AntTweakBar.h>				// |AntWeakBar

#include "Test.h"
using namespace glm;				// NAMESPACE

GLFWwindow* window;
float SPEED;
float ANGLE;

bool initALL();
bool initGLFW();
bool initGLEW();

bool initALL() {
	return initGLFW();
}

bool initGLFW() {
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(16*100, 9*100, "Tutorial 6650 - Keyboard and Mouse", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);



	return initGLEW();
}

bool initGLEW() {
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	return true;
}

btDiscreteDynamicsWorld* initBullet() {
	//==============================================================================================================================================
	// Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	// The world.
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -0.01, 0));

	return dynamicsWorld;
}

int main(void)
{

	bool isInit = initALL();
	btDiscreteDynamicsWorld* dynamicsWorld = initBullet();

	// GROUND
	std::vector<btRigidBody*> rigidbodies;
	btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	btCollisionShape* groundShape = new btBoxShape(btVector3(50.0f, 50.0f, 50.0f));
	btDefaultMotionState* groundMS = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(.0f, -56.f, 0.f)));
	btRigidBody::btRigidBodyConstructionInfo groundCI(0, groundMS, groundShape, btVector3(0, 0, 0));
	btRigidBody *ground = new btRigidBody(groundCI);
	dynamicsWorld->addRigidBody(ground);
	rigidbodies.push_back(ground);


	// GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(1024, 768);
	TwBar * GUI = TwNewBar("Picking");


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");



	// ======================== DESSIN ============================
	glm::mat4 viewMat = getViewMatrix();
	float fps = 1;
	TwAddVarRW(GUI, "camera", TW_TYPE_QUAT4F, &viewMat, "label='viewMatrix' opened=true ");
	char * path = "C:/Users/emili/Downloads/chevre.jpg";
	Test square1 = Test();
	square1.init(path);
	// ======================== /DESSIN ===========================



	// BOB ROSS CUBIC SPHERE
	glm::vec3 positions = glm::vec3(0, 0, 0);
	glm::quat orientations = glm::normalize(glm::quat(glm::vec3(0, 0, 0)));

	btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
		btQuaternion(orientations.x, orientations.y, orientations.z, orientations.w),
		btVector3(positions.x, positions.y, positions.z)
	));

	btCollisionShape* boxCollisionShape2 = new btSphereShape(1);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		1,                  // mass, in kg. 0 -> Static object, will never move.
		motionstate,
		boxCollisionShape2,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
	);
	btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);
	rigidbodies.push_back(rigidBody);
	dynamicsWorld->addRigidBody(rigidBody);


	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {

		btVector3 p0 = rigidbodies[0]->getCenterOfMassPosition();
		glm::vec3 v0 = positions;
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		double currentTime = glfwGetTime();

		rigidbodies[1]->activate(true);
		rigidbodies[1]->setAngularVelocity(btVector3(SPEED, ANGLE, 0.00));


		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}
		float deltaTime = currentTime - lastTime;

		dynamicsWorld->stepSimulation(deltaTime, 14);

		btScalar matrix[16];
		btTransform trans = rigidbodies[1]->getWorldTransform();
		trans.getOpenGLMatrix(matrix);
		glm::mat4 zjfzojf = glm::mat4(
			matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]);


		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * zjfzojf;
		viewMat = ViewMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		square1.draw();


		trans = rigidbodies[0]->getWorldTransform();
		trans.getOpenGLMatrix(matrix);
		zjfzojf = glm::mat4(
			matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]);
		zjfzojf = glm::scale(zjfzojf, glm::vec3(50, 50, 50));
		MVP = ProjectionMatrix * ViewMatrix * zjfzojf;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		square1.draw();

		TwDraw();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glDeleteProgram(programID);
	glfwTerminate();

	return 0;
}