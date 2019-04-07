#pragma once
#include <glfw3.h>						// |GLFW

class SControls
{
public:
	static SControls& getInstance() {
		static SControls instance;
		return instance;
	}

	bool init(int width, int height);
	GLFWwindow* getWindows();
	void computeMatrices();

private:
	SControls() = default;
	~SControls() = default;
	SControls(const SControls&) = delete;
	SControls& operator=(const SControls&) = delete;
	
	GLFWwindow* WINDOW;

	int WIDTH;
	int HEIGHT;
};
