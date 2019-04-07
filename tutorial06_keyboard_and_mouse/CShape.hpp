#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "CShape.hpp"


class CShape {
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;

	GLuint VAO;

	CShape();
	~CShape();

	void Init(std::vector<glm::vec3> & vertices, std::vector<glm::vec2> & uvs, std::vector<unsigned int> & elements);
	void Draw();

private:
	GLuint vertexBO;
	GLuint uvBO;
	GLuint indicesBO;


};

