#pragma once
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>


class Test
{
public:
	Test();
	~Test()=default;

	void init(GLfloat * vertices, GLuint size_vertices, GLuint * indices, GLuint size_indices, GLfloat * uv, GLuint size_uv, char * path);
	void setup();
	void draw();
	void denit();
	glm::mat4 getModel();
	void translateX(float x);

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

