#include "CShape.hpp"


CShape::CShape() {
}


CShape::~CShape() {
}


void CShape::Init(std::vector<glm::vec3> & vertices, std::vector<glm::vec2> & uvs, std::vector<unsigned int> & elements) {
	this->vertices = vertices;
	this->uvs = uvs;
	this->indices = elements;

	glGenVertexArrays(1, &(this->VAO));

	glGenBuffers(1, &this->vertexBO);
	glGenBuffers(1, &this->uvBO);
	glGenBuffers(1, &this->indicesBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBO);
	glBufferData(GL_ARRAY_BUFFER, (vertices).size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->uvBO);
	glBufferData(GL_ARRAY_BUFFER, (uvs).size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (elements).size() * sizeof(unsigned short), &elements[0], GL_STATIC_DRAW);
}

void CShape::Draw() {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBO);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, this->uvBO);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesBO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_SHORT, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
