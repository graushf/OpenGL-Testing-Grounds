#ifndef CAMERAFIRSTPERSON_H
#define CAMERAFIRSTPERSON_H

	#include "Camera.h"

	class CameraFirstPerson : public Camera{
		public:
			glm::vec3 ProjectedFront;

			// Constructor with vectors
			CameraFirstPerson();
			CameraFirstPerson(glm::vec3 position);
			CameraFirstPerson(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch);
			// Constructor with scalar values
			CameraFirstPerson(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

			void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime, bool shift); // ATTENTION


		private:
			void updateCameraVectors() override;
			void getProjectedFrontVector();
	};




#endif /* CAMERAFIRSTPERSON_H */