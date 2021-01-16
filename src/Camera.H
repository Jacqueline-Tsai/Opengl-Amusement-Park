#ifndef GAMERA_H
#define GAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Global.H"

class Camera {
public:
	Camera();
	Camera(float _width, float _height, float _fieldOfView = PI / 2, float _farView = 2000, float _nearView = 0.001);
	void use();
	virtual int handle(int);	

	float speed = 2;
	int mouseDown = 0; //0:none, 1:left, 2:right
	glm::vec2 mouse;
	float fieldOfView, width, height, farView, nearView;

	glm::vec3 Camera::test(float x, float y,glm::vec3 lookAt);
};

#endif