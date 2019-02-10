// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

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
	window = glfwCreateWindow(1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
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

	// Create the VAO
	GLuint VertexArrayID[1];
	glGenVertexArrays(1, VertexArrayID);
	glBindVertexArray(VertexArrayID[0]);

	// Create the VBOs
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);


	// DONNEES STATIQUES
	static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	 0.0f,  1.0f, 0.0f,
	 1.0f,  0.0f, 0.0f
	};

	// Binding 1
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	float lol = 3.0f;
	float lol2 = 0.0f;

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	// DEBUT BOUUUUUUUUUUUCLLLLLLLEEE
	do {
		lol2 += 0.001;
		lol += 10;
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, vec3(0.5*glm::sin(lol2), 0.0, 0.0));
	trans = glm::rotate(trans, glm::radians(lol), glm::vec3(0.0, 1.0, 0.0));

	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 1.0));
	unsigned int transformLoc = glGetUniformLocation(programID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 5); // 3 indices starting at 0 -> 1 triangle


	// Swap buffers
	glfwSwapBuffers(window);
	
		glfwPollEvents();
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}
	}
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// CLEANUP
	glDisableVertexAttribArray(0);				//	VAO
	glDeleteBuffers(2, &vertexbuffer);			//	VBO
	glDeleteVertexArrays(1, VertexArrayID);		//	VAO
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
