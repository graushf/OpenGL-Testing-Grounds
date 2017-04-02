
# include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

bool toggleWireframe = false;
// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const GLchar* vertexShaderSource_triangle2 = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource_triangle2 = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";


int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	//Init GLFW
	glfwInit();
	if (!glewInit()) {
		std::cout << "Couldn't initialize GLFW, glewInit() Failed";
		return -1;
	}

	// Set all the required options for GLFW
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwDefaultWindowHints();

	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maixmum nr of vertex attributes supported: " << nrAttributes << std::endl;


	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Project4_Exercise1", nullptr , nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so that GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup OpenGL function pointers
	glewInit();

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Build and compile our shader program
	// Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Chaeck for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// Link Shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Vertex Shader - Triangle2
	GLuint vertexShaderT2 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderT2, 1, &vertexShaderSource_triangle2, NULL);
	glCompileShader(vertexShaderT2);
	// Check for compile time errors
	GLint successT2;
	GLchar infoLogT2[512];
	glGetShaderiv(vertexShaderT2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderT2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	GLuint fragmentShaderT2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderT2, 1, &fragmentShaderSource_triangle2, NULL);
	glCompileShader(fragmentShaderT2);
	// Check for compile time errors
	glGetShaderiv(fragmentShaderT2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderT2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link Shaders
	GLuint shaderProgramT2 = glCreateProgram();
	glAttachShader(shaderProgramT2, vertexShaderT2);
	glAttachShader(shaderProgramT2, fragmentShaderT2);
	glLinkProgram(shaderProgramT2);
	// Cheack for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShaderT2);
	glDeleteShader(fragmentShaderT2);





	// Set up vertex data (and buffer(s) and attribute pointers) for triangle 1
	GLfloat vertices_triangle1[] = {
		-0.25f, 0.5f, 0.0f,  // Top 
		-0.50f, -0.5f, 0.0f,  // Left
		-0.0f, -0.5f, 0.0f  // Right
	};

	GLuint indices_triangle1[] = { // Note that we start from 0!
		0, 1, 2 // Triangle
	};

	GLuint VBO_T1, VAO_T1, EBO_T1;
	glGenVertexArrays(1, &VAO_T1);
	glGenBuffers(1, &VBO_T1);
	glGenBuffers(1, &EBO_T1);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s)
	glBindVertexArray(VAO_T1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_T1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle1), vertices_triangle1, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_T1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangle1), indices_triangle1, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registers VBO as the currentlybould vertex buffer object so afterwards we can safely unbind.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO.



	// Set up vertex data (and buffer(s) and attribute pointers) for triangle 2
	GLfloat vertices_triangle2[] = {
		0.0f, -0.5f, 0.0f,
		0.50f, -0.5f, 0.0f,
		0.25f, 0.5f, 0.0f
	};

	GLuint indices_triangle2[] = {
		0, 1, 2
	};

	GLuint VBO_T2, VAO_T2, EBO_T2;
	glGenVertexArrays(1, &VAO_T2);
	glGenBuffers(1, &VBO_T2);
	glGenBuffers(1, &EBO_T2);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s)
	glBindVertexArray(VAO_T2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_T2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle2), vertices_triangle2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_T2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangle2), indices_triangle2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);




	// Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activated (key pressed, mouse moved, etc. ) and call corresponding response functions.
		glfwPollEvents();


		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (toggleWireframe == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		// Draw our first triangle
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO_T1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		glUseProgram(shaderProgramT2);

		glBindVertexArray(VAO_T2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO_T1);
	glDeleteBuffers(1, &VBO_T1);
	glDeleteBuffers(1, &EBO_T1);
	glDeleteVertexArrays(1, &VAO_T2);
	glDeleteBuffers(1, &VBO_T2);
	glDeleteBuffers(1, &EBO_T2);
	// Terminate GLFW, clearing any resources allocated by GLFW
	glfwTerminate();
	return 0;
}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		toggleWireframe = !toggleWireframe;
	}
}