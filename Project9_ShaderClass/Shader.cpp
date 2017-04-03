#include "Shader.h"

#include <vector>
#include <cassert>
/*
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try {
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;	
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATRION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	// Print linking errors if any
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as tehy're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
*/

std::string loadTextFile(const std::string& filename) {
	std::stringstream buffer;
	buffer << std::ifstream(filename.c_str()).rdbuf();
	return buffer.str();
}


GLuint compileShaderStage(GLenum stage, const std::string& source)
{
	GLuint shader = glCreateShader(stage);
	const char* srcArray[] = { source.c_str() };

	glShaderSource(shader, 1, srcArray, NULL);
	glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<GLchar> erroLog(logSize);
		glGetShaderInfoLog(shader, logSize, &logSize, &erroLog[0]);
		assert(false);

		glDeleteShader(shader);
		shader = GL_NONE;
	}
	return shader;
}



Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string sVertexShaderSource;
	std::string sFragmentShaderSource;

	sVertexShaderSource = loadTextFile(vertexPath);
	sFragmentShaderSource = loadTextFile(fragmentPath);

	std::cout << "out" << std::endl;

	GLuint vertexShader = compileShaderStage(GL_VERTEX_SHADER, sVertexShaderSource);
	GLuint fragmentShader = compileShaderStage(GL_FRAGMENT_SHADER, sFragmentShaderSource);

	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertexShader);
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);
	GLint success;
	GLchar infoLog[512];
	// Print linking errors if any
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as theyy're linked into our program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void Shader::Use() {
	glUseProgram(this->Program);
}