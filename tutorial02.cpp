// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

#include "Test.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;



#include <common/shader.hpp>

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "23 MILFS localisees pres de chez toi !", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	// ====================  DEBUT  ====================== //
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	glUseProgram(programID);
	static GLfloat g_vertex_buffer_data[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f,  0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
	};

	static GLuint indices[] = {
	0,1,2,
	0,3,2
	};

	float texCoords[] = {
	0.0f, 0.0f, 
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	};


	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, -60), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, -1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(90.0f), 4/3.f, 0.1f, 100.0f);



	Test test = Test();
	test.init(
		g_vertex_buffer_data, sizeof(g_vertex_buffer_data),
		indices, sizeof(indices),
		texCoords, sizeof(texCoords),
		"C:/Users/jules/Desktop/eaze/AAA/bobross.jpg"
	);


	do {
		glClear(GL_COLOR_BUFFER_BIT);

		test.translateX(0.0001);
		glm::mat4 mvp = projection * view * test.getModel();

		GLuint MatrixID = glGetUniformLocation(programID, "MVP");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		test.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	test.denit();
	glDeleteProgram(programID);
	glfwTerminate();
	return 0;
}
