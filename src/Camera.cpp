#include <cmath>
#include <iostream>
#include <windows.h>
#include <FL/Fl_Gl_Window.h>
#include <Fl/Fl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Fl/Fl_Double_Window.h>
#include "Camera.H"

#define PI 3.14159

const glm::mat3 rotate_y_90{ 0,0,1,  0,1,0, -1,0,0 };

Camera::Camera() {

}

Camera::Camera(float _width, float _height, float _fieldOfView, float _farView, float _nearView)
	:fieldOfView(_fieldOfView), width(_width), height(_height), farView(_farView), nearView(_nearView){

}

void Camera::use() {
	if (Global::camera_mode == 0) {
		Global::position = Global::position_ground;
		Global::lookAt = glm::normalize(Global::lookAt_ground);
		Global::up = Global::up_ground;
	}
	else if (Global::camera_mode == 1) {
		Global::position = Global::position_ferris_wheel;
		Global::lookAt = glm::normalize(Global::lookAt_ferris_wheel);
		Global::up = Global::up_ferris_wheel;
	}
	else if (Global::camera_mode == 2) {
		Global::position = Global::position_roller_coaster;
		Global::lookAt = glm::normalize(Global::lookAt_roller_coaster);
		Global::up = Global::up_roller_coaster;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView * 90 / PI, width / height, nearView, farView);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Global::position.x, Global::position.y, Global::position.z,
		Global::position.x + Global::lookAt.x, Global::position.y + Global::lookAt.y, Global::position.z + Global::lookAt.z,
		Global::up.x, Global::up.y, Global::up.z);
}
glm::vec3 Camera::test(float x, float y, glm::vec3 lookAt) {
	x /= width;y /= height;
	glm::vec3 ans = glm::normalize(lookAt);
	float degree = (x)*fieldOfView;
	glm::mat3 rotate_y{ cos(degree),0,sin(degree),  0,1,0, -sin(degree),0,cos(degree) };
	ans = rotate_y * ans;

	degree = (-y)*fieldOfView;
	glm::mat3 rotate_x{ 1,0,0,  0,cos(degree),-sin(degree), 0,sin(degree),cos(degree) };
	ans.y = (rotate_x * glm::vec3(0, ans.y, sqrt(1 - ans.y * ans.y))).y;
	return ans;
}
int Camera::handle(int event) {
	glm::vec3 position, lookAt, up;
	if (Global::camera_mode == 0) {
		position = Global::position_ground;
		lookAt = glm::normalize(Global::lookAt_ground);
		up = Global::up_ground;
	}
	else if (Global::camera_mode == 1) {
		position = Global::position_ferris_wheel;
		lookAt = glm::normalize(Global::lookAt_ferris_wheel);
		up = Global::up_ferris_wheel;
	}
	else if (Global::camera_mode == 2) {
		position = Global::position_roller_coaster;
		lookAt = glm::normalize(Global::lookAt_roller_coaster);
		up = Global::up_roller_coaster;
	}

	switch (event) {
	case FL_PUSH:
		if (mouseDown == 0) {
			if (Fl::event_button() == FL_LEFT_MOUSE) mouseDown = 1;
			if (Fl::event_button() == FL_RIGHT_MOUSE) mouseDown = 2;
			mouse.x = Fl::event_x();
			mouse.y = Fl::event_y();
		}		
		break;
	case FL_RELEASE:
		mouseDown = 0;
		break;
	case FL_DRAG:
		if (mouseDown == 1 && Fl::event_button() == FL_LEFT_MOUSE) {
			float xx = Fl::event_x() - mouse.x, yy = Fl::event_y() - mouse.y;
			lookAt = test(xx, yy, lookAt);
			mouse.x = Fl::event_x();
			mouse.y = Fl::event_y();
			if (Global::camera_mode == 0)  Global::lookAt_ground = lookAt;
			else if (Global::camera_mode == 1)Global::lookAt_ferris_wheel = lookAt;
		}
		break;
	case FL_MOUSEWHEEL:
		break;
	case FL_KEYBOARD: //keyboard
		glm::vec3 move;
		if (Fl::event_key() == int('E') || Fl::event_key() == int('e')) {
			move = glm::vec3(0, 1, 0);
		}
		else if (Fl::event_key() == int('Q') || Fl::event_key() == int('q')) {
			move = glm::vec3(0, -1, 0);
		}
		else if (Fl::event_key() == int('W') || Fl::event_key() == int('w')) {
			move = lookAt;	move.y = 0;
		}
		else if (Fl::event_key() == int('A') || Fl::event_key() == int('a')) {
			move = -rotate_y_90 * lookAt;	move.y = 0;
		}
		else if (Fl::event_key() == int('S') || Fl::event_key() == int('s')) {
			move = -lookAt;	move.y = 0;	move.y = 0;
		}
		else if (Fl::event_key() == int('D') || Fl::event_key() == int('d')) {
			move = rotate_y_90 * lookAt;	move.y = 0;
		}
		else {
			return 1;
		}
		position += glm::normalize(move) * speed;
		if (Global::camera_mode == 0) {
			Global::position_ground = position;
			if (!Global::debug_mode) {
				Global::position_ground.x = fmax(-480, fmin(position.x, 480));	position.z = fmax(-480, fmin(position.z, 480));
				Global::position_ground.y = Global::get_terrain_height(position.x, position.z) + 10;
			}
		}
		break;
	}
	return 1;
}