
#include <iostream>


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "input.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;


int main() {
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all thre required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Project1_Basic-Window", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	Input* input = new Input();

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check and call events
		glfwPollEvents();

		// Render
		// Clear the colorbufer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Swap the buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/relesased via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}




