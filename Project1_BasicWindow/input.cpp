#include "input.h"
#include <iostream>

Input::Input() {
	// Setting up Input constructor
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}