#ifndef GLOBAL_H
#define GLOBAL_H
#include <glm/glm.hpp>
#include "Control.H"

#include <ctime>

#define PI 3.14159

class Control;

class Global {
public:
	static void update();
	static float get_terrain_height(float posX, float posZ);

	// camera
	static int camera_mode;
	static glm::vec3 position;
	static glm::vec3 lookAt;
	static glm::vec3 up;
	static glm::vec3 position_ground;
	static glm::vec3 lookAt_ground;
	static glm::vec3 up_ground;
	static glm::vec3 position_ferris_wheel;
	static glm::vec3 lookAt_ferris_wheel;
	static glm::vec3 up_ferris_wheel;
	static glm::vec3 position_roller_coaster;
	static glm::vec3 lookAt_roller_coaster;
	static glm::vec3 up_roller_coaster;

	// sin wave
	static float speed;
	static float amplitude;
	static float waveLen;
	static float height;
	
	// terrain
	static float offset_max;
	static float d_offset;
	static int terrain_lv;
	static glm::mat4 terrain_model_matrix;
	static float terrain_height[193][193];

	// Ferris Wheel
	static int ferris_wheel_cars_number;
	static int ferris_wheel_max_lv;
	static float ferris_wheel_speed;
	static glm::mat4 ferris_wheel_model_matrix;

	// roller coaster
	static glm::mat4 roller_coaster_model_matrix;

	static bool debug_mode;

	//render
	static bool colorBlock;
	static bool pencil;

	static Control *control;

	// Time
	// Cautions: It's unit is SECOND. 
	static float getCurrentTime() {
		return (float)(clock()) * 0.001f;
	}
	static clock_t time;
	static float timeInterval;
	
};



#endif