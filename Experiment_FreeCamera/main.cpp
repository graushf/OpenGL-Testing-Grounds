/*
*	Exercise17
*	Try to create your own LookAt function where you manually create a view matrix 
*	as discussed at the start of this tutorial. Replace glm's LookAt function with 
*	your own implementation and see if it still acts the same.
*/


// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL Includes
#include "Shader.h"
#include "Camera.h"


// GL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processMouse(int state);
void Do_Movement();

// Camera
//Camera camera(glm::vec3(0.0f, 3.0f, 3.0f));
//Camera camera(glm::vec3(0.0f, 2.5f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0f);
glm::vec3 initial_pos_camera = glm::vec3(3.08106f, 1.55655f, 3.20902f);
Camera camera(initial_pos_camera, glm::vec3(0.0f, 1.0f, 0.0f), -134.5f, -18.75f);

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat xoffset = 0;
GLfloat yoffset = 0;
GLfloat xoffset2 = 0;
GLfloat yoffset2 = 0;
bool rotateScene = false;
bool activePanScene = false;
glm::mat4 rotationMouseMatrixHorizontal = glm::mat4();
glm::mat4 rotationMouseMatrixVertical = glm::mat4();
glm::vec2 cameraPan = glm::vec2(0.0f, 0.0f);
glm::mat4 cameraPanMatrix = glm::mat4();
GLfloat lastXpan = screenWidth / 2;
GLfloat lastYpan = screenHeight / 2;
GLfloat xoffsetPan = 0;
GLfloat yoffsetPan = 0;
// The MAIN function, from here we start out application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwDefaultWindowHints();

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Project36_Exercise17", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup and compile our shaders
	Shader ourShader("./shaders/shader.vertex", "./shaders/shader.frag");
	Shader planeShader("./shaders/shaderPlane.vertex", "./shaders/shaderPlane.frag");
	Shader backgroundShader("./shaders/shaderBackground.vertex", "./shaders/shaderBackground.frag");

	

	
	GLfloat verticesPlane[] = {
		 0.5f,	0.0f,  0.5f,
		 0.5f,	0.0f, -0.5f,
		-0.5f,	0.0f,  0.5f,


		 0.5f,	0.0f, -0.5f,
		-0.5f,	0.0f, -0.5f,
		-0.5f,	0.0f,  0.5f
	};

	GLfloat textureCoordinatesPlane[] = {
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	GLuint planeVBO, texCoordsPlaneVBO, planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &texCoordsPlaneVBO);
	
	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlane), verticesPlane, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsPlaneVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinatesPlane), textureCoordinatesPlane, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);	// Unbind VAO



	GLfloat verticesBackground[] = {
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,

		 1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};

	GLfloat textureCoordinatesBackground[] = {
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	GLuint backgroundVBO, backgroundTextureCoordsVBO, backgroundVAO;
	glGenVertexArrays(1, &backgroundVAO);
	glGenBuffers(1, &backgroundVBO);
	glGenBuffers(1, &backgroundTextureCoordsVBO);

	glBindVertexArray(backgroundVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBackground), verticesBackground, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	

	glBindBuffer(GL_ARRAY_BUFFER, backgroundTextureCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinatesBackground), textureCoordinatesBackground, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);


	// Load and create a texture
	GLuint textureBackground;
	// --== TEXTURE 1 == --
	glGenTextures(1, &textureBackground);
	glBindTexture(GL_TEXTURE_2D, textureBackground);	// All upcoming GL_TEXTURE_2D operations now have effect on our texture object
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width = 0;
	int height = 0;
	unsigned char* image_background = SOIL_load_image("./resources/texture_background.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_background);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_background);
	glBindTexture(GL_TEXTURE_2D, 0);	// Unbind texture when done, so we won't accidentily mess up our texture.
	
	// --== TEXTURE 2 == --
	GLuint texturePlane;
	// --== TEXTURE 2 ==--
	glGenTextures(1, &texturePlane);
	glBindTexture(GL_TEXTURE_2D, texturePlane);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	width = 0;
	height = 0;
	unsigned char* image_plane = SOIL_load_image("./resources/plane_texture2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_plane);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_plane);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentaly mess up our texture.
	
	cameraPanMatrix = camera.calculate_lookAt_matrix(glm::vec3(initial_pos_camera.x, initial_pos_camera.y, initial_pos_camera.z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Game Loop
	while (!glfwWindowShouldClose(window))
	{

		

		//camera.updateOutputCamera();
		// Set frame rate
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Mouse state controls
		// Middle mouse state control
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
		processMouse(state);

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		//glDisable(GL_DEPTH);
		backgroundShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBackground);
		GLint aux2 = glGetUniformLocation(backgroundShader.Program, "ourTexture");
		glUniform1i(glGetUniformLocation(backgroundShader.Program, "ourTexture"), 0);
		glBindVertexArray(backgroundVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		//glEnable(GL_DEPTH);
		
		glEnable(GL_DEPTH_TEST);

		planeShader.Use();
		// Bind Textures using texture units 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePlane);
		glUniform1i(glGetUniformLocation(planeShader.Program, "ourTexture"), 0);

		// Create camera transformation
		glm::mat4 view;
		//view = camera.calculate_lookAt_matrix(glm::vec3(initial_pos_camera.x, initial_pos_camera.y, initial_pos_camera.z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		if (activePanScene == true) {
			cameraPan.x =  0.05*(cameraPan.x + xoffsetPan);
			cameraPan.y =  0.05*(cameraPan.y + yoffsetPan);
		}
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 horizVec = glm::normalize(right + front);
		glm::vec3 movHorizontal = cameraPan.x * horizVec;
		glm::vec3 movVertical = cameraPan.y * glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 vectorPanCamera = initial_pos_camera + (movHorizontal + movVertical);
		cameraPanMatrix = camera.calculate_lookAt_matrix_freecam(vectorPanCamera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//cameraPanMatrix = camera.calculate_lookAt_matrix_freecam(glm::vec3(initial_pos_camera.x + cameraPan.x, initial_pos_camera.y, initial_pos_camera.z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = camera.calculate_lookAt_matrix(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = camera.calculate_lookAt_matrix(glm::vec3(cameraPan.x, cameraPan.y, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//std::cout << "cameraPan.x = " << cameraPan.x << "\t\t cameraPan.y = " << cameraPan.y << std::endl;
		view = cameraPanMatrix;
		
		glm::mat4 projection;
		projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(planeShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(planeShader.Program, "view");
		GLint projLoc = glGetUniformLocation(planeShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model = glm::mat4();
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		if (rotateScene == true) {
			//std::cout << "MOUSE xoffset:\t\t" << xoffset << std::endl;
			//std::cout << "MOUSE yoffset:\t\t" << yoffset << std::endl;
			rotationMouseMatrixHorizontal = glm::rotate(rotationMouseMatrixHorizontal, (float)0.1*glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 vecRotation = glm::normalize(right + front);
			rotationMouseMatrixVertical = glm::rotate(rotationMouseMatrixVertical, -(float)0.1*glm::radians(yoffset), vecRotation);
			std::cout << "MOUSE radiansRotations:\t\t" << 0.1 * glm::radians(xoffset) << std::endl;
		}
		model = rotationMouseMatrixVertical * rotationMouseMatrixHorizontal * model;
		//model = rotationMouseMatrixVertical * model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		

		// Swap the buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they-ve outlived their purpose
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &texCoordsPlaneVBO);
	glDeleteVertexArrays(1, &backgroundVAO);
	glDeleteBuffers(1, &backgroundVBO);
	glDeleteBuffers(1, &backgroundTextureCoordsVBO);
	glfwTerminate();
	return 0;
}


// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera Controls
	if (keys[GLFW_KEY_W]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}


// Is called whenever a key is pressed/released bia GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}

	if (key == GLFW_KEY_SPACE) {
		std::cout << "Stop" << std::endl;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	/*
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	xoffset = xpos - lastX;
	yoffset = lastY - ypos;		// Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;
	*/
	// Mouse callback Free camera:
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	xoffset = xpos - lastX;
	yoffset = lastY - ypos;		// Reversed since y-coordinates go from bottom to left

	//if (rotateScene == false) {
		lastX = xpos;
		lastY = ypos;
	//}

	xoffsetPan = xpos - lastXpan;
	yoffsetPan = lastYpan - ypos;

	if (activePanScene == false) {
		lastXpan = xpos + cameraPan.x;
		lastYpan = ypos + cameraPan.y;
	}

	//camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		//std::cout << "Right Mouse Button pressed" << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//std::cout << "Left Mouse Button pressed" << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		//std::cout << "Middle Mouse Button pressed" << std::endl;
	}
}


void processMouse(int state) {
	if (state == GLFW_PRESS) {
		if (keys[GLFW_KEY_LEFT_ALT] || keys[GLFW_KEY_RIGHT_ALT]) {
			std::cout << "Middle mouse button + Alt is being pressed" << std::endl;
			//camera.ProcessMouseMovementRotation(xoffset, yoffset, true);
			rotateScene = true;
		}
		else {
			rotateScene = false;
		}
		if (keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT]) {
			//std::cout << "Middle mouse button + Shift is being pressed" << std::endl;
			activePanScene = true;
		} 
		else {
			activePanScene = false;
		}
	}
	else {
		rotateScene = false;
		activePanScene = false;
	}
	
}