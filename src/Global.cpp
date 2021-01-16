#include "Global.H"

Control* Global::control = nullptr;

glm::vec3 Global::position;
glm::vec3 Global::lookAt;
glm::vec3 Global::up;
glm::vec3 Global::position_ground = glm::vec3(0, 0, 0);
glm::vec3 Global::lookAt_ground = glm::vec3(0, 0, 1);
glm::vec3 Global::up_ground = glm::vec3(0, 1, 0);
glm::vec3 Global::position_ferris_wheel = glm::vec3(0, 0, 0);
glm::vec3 Global::lookAt_ferris_wheel = glm::vec3(0, 0, 1);
glm::vec3 Global::up_ferris_wheel = glm::vec3(0, 1, 0);
glm::vec3 Global::position_roller_coaster = glm::vec3(0, 0, 0);
glm::vec3 Global::lookAt_roller_coaster = glm::vec3(0, 0, 1);
glm::vec3 Global::up_roller_coaster = glm::vec3(0, 1, 0);
int Global::camera_mode = 0; //0-ground, 1-ferris wheel

float Global::speed = 5;
float Global::amplitude = 2;
float Global::waveLen = 20;
float Global::height = 10;

float Global::offset_max = 0.4;
float Global::d_offset = 0.4;
int Global::terrain_lv = 6;
glm::mat4 Global::terrain_model_matrix= glm::mat4(320, 0, 0, 0, 0, 100, 0, 0, 0, 0, 320, 0, -480, 0, -480, 1);
float Global::terrain_height[193][193];

int Global::ferris_wheel_cars_number = 5;
float Global::ferris_wheel_speed = 0.3;
int Global::ferris_wheel_max_lv = 4;
glm::mat4 Global::ferris_wheel_model_matrix = glm::mat4(100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 160, 20, 160, 1);

glm::mat4 Global::roller_coaster_model_matrix = glm::mat4(100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 160, 40, -160, 1);

bool Global::debug_mode = false;
bool Global::colorBlock = false;
bool Global::pencil = false;

clock_t Global::time = 0;
float Global::timeInterval = 0;

void Global::update() {
	//offset_max = control->offset_max->value();
	//d_offset = control->d_offset->value();
	terrain_lv = round(control->terrain_lv->value());

	ferris_wheel_speed = control->ferris_wheel_speed->value();

	debug_mode = control->debug_mode->value();
	colorBlock = control->colorBlock->value();
	pencil = control->pencil->value();

	if (control->camGround->value())camera_mode = 0;
	else if (control->camFerris->value())camera_mode = 1;
	else if (control->camRoller->value())camera_mode = 2;
	timeInterval = (clock() - time) / 1000.0;
	time = clock();
}

float Global::get_terrain_height(float posx, float posz) {
	float offset = terrain_model_matrix[3][0];int ratio = terrain_model_matrix[0][0] / 64;
	return terrain_height[int(posx - offset) / ratio][int(posz - offset) / ratio] * terrain_model_matrix[1][1];
}

