#pragma once
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION

struct Vertex {
	glm::vec3 Vertices;
	glm::vec2 UVs;
	glm::vec3 Normals;
};

class Test {
public:
	Test();
	~Test() = default;

	void init(char * path);

	void setup();
	void draw();
	void denit();
	glm::mat4 getModel();
	void translateX(float x);
	void rotateZ(float x);
	void rotateY(float y);
	void scale(float s);

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint UVO;
	GLuint TextureID;

	GLfloat * vertices;
	GLuint * indices;
	GLfloat * uv;
	char * path;

	glm::mat4 model = glm::mat4(1.0f);
};