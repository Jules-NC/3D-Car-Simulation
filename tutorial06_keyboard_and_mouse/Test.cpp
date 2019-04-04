#include <iostream>
#include "Test.h"

#include "stb_image.h"
#include <assimp/Importer.hpp>			// |Assimp
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h> 

Test::Test() {}

void Test::init(char * path) {

	Assimp::Importer importer;
	std::string obj = "C:/Users/emili/OneDrive/Documents/square.obj";
	const aiScene* scene = importer.ReadFile(obj, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene) {
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return;
	}
	const aiMesh * mesh = scene->mMeshes[0];
	GLuint numFaces = mesh->mNumFaces;
	GLuint numVertices = mesh->mNumVertices;
	aiVector3D * vertices = mesh->mVertices;

	GLfloat * myVertices = (float *)malloc(numVertices * sizeof(float) * 3);
	GLuint * myElements = (unsigned int *)malloc(numFaces * sizeof(unsigned int) * 3);

	const aiFace * faces = mesh->mFaces;

	std::cout << "VERTICES" << std::endl;
	int j = 0;
	for (int i = 0; i < numVertices; i++) {
		myVertices[j] = vertices[i].x;
		myVertices[j + 1] = vertices[i].y;
		myVertices[j + 2] = vertices[i].z;
		j += 3;
	}

	j = 0;
	std::cout << "\nINDICES" << std::endl;
	for (int i = 0; i < numFaces; i++) {
		GLuint mIndices = faces[i].mNumIndices;
		GLuint * indices = faces[i].mIndices;

		myElements[j] = indices[0];
		myElements[j + 1] = indices[1];
		myElements[j + 2] = indices[2];
		j += 3;
	}

	for (int i = 0; i < numFaces * 3; i++) {
		//std::cout << "TESST: " << i << " - " << myElements[i] << "\n";
	}



	this->vertices = myVertices;
	this->indices = myElements;
	this->uv = myVertices;
	this->path = path;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &UVO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 3, this->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numFaces * 3, this->indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, UVO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 3, this->uv, GL_STATIC_DRAW);

	this->setup();
}


void Test::setup() {
	glGenTextures(1, &this->TextureID);
	glBindTexture(GL_TEXTURE_2D, this->TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(this->path, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Load image SUCCESS" << std::endl;
		std::cout << "Image size : " << width << ", " << height << std::endl;
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Test::draw() {
	glBindTexture(GL_TEXTURE_2D, this->TextureID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);
	glDrawElements(GL_TRIANGLES, 3000, GL_UNSIGNED_INT, 0);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Test::denit() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &UVO);
	glDeleteVertexArrays(1, &VAO);
}

glm::mat4 Test::getModel()
{
	return this->model;
}

void Test::translateX(float x) {
	this->model = glm::translate(this->model, glm::vec3(x, 0.0, 0.0));
}

void Test::rotateZ(float x) {
	this->model = glm::rotate(this->model, x, glm::vec3(0.f, 0.f, 1.f));
}

void Test::rotateY(float y) {
	this->model = glm::rotate(this->model, y, glm::vec3(0.f, 1.f, 0.f));
}

void Test::scale(float s) {
	this->model = glm::scale(this->model, glm::vec3(s, s, s));
}