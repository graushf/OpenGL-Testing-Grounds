#include "Camera.h"
#include <iostream>


// Default camera values
const GLfloat YAW					= -90.0f;
const GLfloat PITCH					= 0.0f;
const GLfloat SPEED					= 3.0f;
const GLfloat SENSITIVITY			= 0.25f;
const GLfloat ZOOM					= 45.0f;
const GLfloat SCROLLSENSITIVITY		= 0.05f;



Camera::Camera() {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat yaw = YAW;
	GLfloat pitch = PITCH;
	Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::MovementSpeed = SPEED;
	Camera::MouseSensitivity = SENSITIVITY;
	Camera::Zoom = ZOOM;
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch){
	Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::MovementSpeed = SPEED;
	Camera::MouseSensitivity = SENSITIVITY;
	Camera::Zoom = ZOOM;
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}


Camera::Camera(glm::vec3 position) {
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat yaw = YAW;
	GLfloat pitch = PITCH;
	Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::MovementSpeed = SPEED;
	Camera::MouseSensitivity = SENSITIVITY;
	Camera::Zoom = ZOOM;
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}


Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) {
	Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::MovementSpeed = SPEED;
	Camera::MouseSensitivity = SENSITIVITY;
	Camera::Zoom = ZOOM;
	this->Position = glm::vec3(posX, posY, posZ);
	this->WorldUp = glm::vec3(upX, upY, upZ);
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	


	glm::mat4 lookAt;
	glm::mat4 mat1;
	mat1[0][0] = Right.x;
	mat1[0][1] = Right.y;
	mat1[0][2] = Right.z;
	mat1[0][3] = 0.0f;
	mat1[1][0] = Up.x;
	mat1[1][1] = Up.y;
	mat1[1][2] = Up.z;
	mat1[1][3] = 0.0f;
	mat1[2][0] = Front.x;
	mat1[2][1] = Front.y;
	mat1[2][2] = Front.z;
	mat1[2][3] = 0.0f;
	mat1[3][0] = 0.0f;
	mat1[3][1] = 0.0f;
	mat1[3][2] = 0.0f;
	mat1[3][3] = 1.0f;

	glm::mat4 mat2;
	mat2[0][0] = 1.0f;
	mat2[0][1] = 0.0f;
	mat2[0][2] = 0.0f;
	mat2[0][3] = -Position.x;
	mat2[1][0] = 0.0f;
	mat2[1][1] = 1.0f;
	mat2[1][2] = 0.0f;
	mat2[1][3] = -Position.y;
	mat2[2][0] = 0.0f;
	mat2[2][1] = 0.0f;
	mat2[2][2] = 1.0f;
	mat2[2][3] = -Position.z;
	mat2[3][0] = 0.0f;
	mat2[3][1] = 0.0f;
	mat2[3][2] = 0.0f;
	mat2[3][3] = 1.0f;


	lookAt = mat2 * mat1 ;
	std::cout << "DEBUG" << std::endl;
	return lookAt;
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}


void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD) {
		this->Position += this->Front * velocity;
	}
	if (direction == BACKWARD) {
		this->Position -= this->Front * velocity;
	}
	if (direction == LEFT) {
		this->Position -= this->Right * velocity;
	}
	if (direction == RIGHT) {
		this->Position += this->Right * velocity;
	}
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
	constrainPitch = true;
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen downs't get flipped
	if (constrainPitch) {
		if (this->Pitch > 89.0f) {
			this->Pitch = 89.0f;
		}
		if (this->Pitch < -89.0f) {
			this->Pitch = -89.0f;
		}
	}

	// Update Front, Right and Up Vectors using the updater Euler Angles
	this->updateCameraVectors();
}



void Camera::ProcessMouseScroll(GLfloat yoffset) {
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f) {
		this->Zoom -= yoffset * SCROLLSENSITIVITY;
	}
	if (this->Zoom <= 1.0f) {
		this->Zoom = 1.0f;
	}
	if (this->Zoom >= 45.0f) {
		this->Zoom = 45.0f;
	}
}

void Camera::updateCameraVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);
	// Also re-calculate the Right and Up Vectors
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

