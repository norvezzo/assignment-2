#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

	
	Camera::Camera(float fov,float relationWH, float zNear, float zFar) 
	{
		if(fov > 0) //prerspective
		{
			this->projection =  glm::perspective(fov,relationWH , zNear, zFar);
		}
		else  //ortho
		{
			this->projection = glm::ortho(-1.0f,1.0f,-1.0f,1.0f,zNear,zFar);
		}
		this->near1 = zNear;
		this->far1 = zFar;
		this->fov = fov;
		this->relationWH = relationWH;
}

	void Camera::SetProjection(float fov, float relationWH)
	{
		if( this->fov>0)
		{
			this->projection = glm::perspective(fov,relationWH , near1, far1);	
			this->fov = fov;
		}
		this->relationWH = relationWH;
	}

	void Camera::SetView(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
		this->eye = eye;
		this->view = glm::lookAt(eye, center, up);
	}

	glm::vec3 Camera::getPosition() {
		return eye;
	}

	glm::vec3 Camera::calculateRayDirection(float x, float y, int imageWidth, int imageHeight) const {
		float ndcX = (2.0f * x) / imageWidth - 1.0f; // normalized device coordinates
		float ndcY = 1.0f - (2.0f * y) / imageHeight;
		glm::vec4 rayClip = glm::vec4(ndcX, ndcY, -1.0, 1.0);

		glm::vec4 rayEye = glm::inverse(projection) * rayClip;
		rayEye = glm::vec4(rayEye.x, rayEye.y, -eye.z, 0.0);

		glm::vec4 rayWorld = glm::inverse(view) * rayEye;
		glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

		return rayDirection;
	}

