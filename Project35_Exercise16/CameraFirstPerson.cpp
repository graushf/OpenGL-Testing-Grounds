#include "CameraFirstPerson.h"


GLfloat velocity;




CameraFirstPerson::CameraFirstPerson() {
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

CameraFirstPerson::CameraFirstPerson(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch){
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


CameraFirstPerson::CameraFirstPerson(glm::vec3 position) {
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


CameraFirstPerson::CameraFirstPerson(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) {
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




void CameraFirstPerson::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime, bool shift) {
	velocity = this->MovementSpeed * deltaTime;
	if (shift == true) {
		velocity = 10 * velocity;
	}
	if (direction == FORWARD) {
		this->Position += this->ProjectedFront * velocity;
	}
	if (direction == BACKWARD) {
		this->Position -= this->ProjectedFront * velocity;
	}
	if (direction == LEFT) {
		this->Position -= this->Right * velocity;
	}
	if (direction == RIGHT) {
		this->Position += this->Right * velocity;
	}
}


void CameraFirstPerson::updateCameraVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);

	getProjectedFrontVector();


	// Also re-calculate the Right and Up Vectors
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

void CameraFirstPerson::getProjectedFrontVector() {
	glm::vec3 n = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	n = glm::normalize(n);

	glm::vec3 frontprojected = Front - glm::dot(Front, n) * n;
	ProjectedFront = glm::normalize(frontprojected);
	//std::cout << "[G] ProjectedFront:			x: " << ProjectedFront.x << " y: " << ProjectedFront.y << " z: " << ProjectedFront.z << std::endl;
}
