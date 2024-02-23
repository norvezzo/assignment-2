#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H
//#include <glm/glm.hpp>
#include "MovableGLM.h"

class Camera : public MovableGLM
{
public:
	Camera( float fov,float relathionWH, float zNear, float zFar);

	inline glm::mat4 GetViewProjection() const	{
		return projection;}

	inline float GetAngle(){
		return fov;}

	inline float GetNear(){
		return near1;}

	inline float GetFar(){
		return far1;}

	inline float GetRelationWH(){
		return relationWH;}
	
	void SetProjection(float fov, float relationWH);

	// added methods
	void SetView(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
	glm::vec3 getPosition();
	glm::vec3 calculateRayDirection(float x, float y, int imageWidth, int imageHeight) const;

protected:
private:
	glm::mat4 projection;
	float fov, relationWH;
	float far1,near1;
	// added fields
	glm::mat4 view;
	glm::vec3 eye;
};

#endif
