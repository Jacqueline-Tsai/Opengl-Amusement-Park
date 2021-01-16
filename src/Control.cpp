#include <iostream>
#include <FileSystem>
#include <fstream>
#include <ctime>
#include <Fl/fl.h>
#include <windows.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glu.h>
#include <filesystem>
#include "Control.H"


void cb(Control* tw){
	//printf("fuck");
}
void toggle(Fl_Button* button) {
	button->value(1 - button->value());
}
Control::Control(int x, int y, int w, int h, const char* l) : Fl_Window(x, y, w, h, l) {
	begin();
	{
		// sin wave
		int pty = 5, ptx = 50;
		height = new Fl_Value_Slider(ptx, pty, 140, 20, "Height");
		height->range(0, 30);
		height->value(10);
		height->type(FL_HORIZONTAL);
		pty += 30;
		amplitude = new Fl_Value_Slider(ptx, pty, 140, 20, "Amplitude");
		amplitude->range(1, 5);
		amplitude->value(2);
		amplitude->type(FL_HORIZONTAL);
		pty += 30;
		waveLen = new Fl_Value_Slider(ptx, pty, 140, 20, "wave len");
		waveLen->range(10, 50);
		waveLen->value(20);
		waveLen->type(FL_HORIZONTAL);
		pty += 30;
		waveSpd = new Fl_Value_Slider(ptx, pty, 140, 20, "wave speed");
		waveSpd->range(0, 5);
		waveSpd->value(0.5);
		waveSpd->type(FL_HORIZONTAL);

		// terrain
		pty = 5; ptx = 200;
		d_offset = new Fl_Value_Slider(ptx, pty, 140, 20, "d offset");
		d_offset->range(0.3, 1);
		d_offset->value(0.8);
		d_offset->type(FL_HORIZONTAL);
		pty += 30;
		offset_max = new Fl_Value_Slider(ptx, pty, 140, 20, "offset max");
		offset_max->range(0, 100);
		offset_max->value(20);
		offset_max->type(FL_HORIZONTAL);
		pty += 30;
		terrain_lv = new Fl_Value_Slider(ptx, pty, 140, 20, "level");
		terrain_lv->range(0, 6);
		terrain_lv->value(6);
		terrain_lv->type(FL_HORIZONTAL);
		
		// Ferris Wheel
		pty = 5; ptx = 350;
		ferris_wheel_cars_number = new Fl_Value_Slider(ptx, pty, 140, 20, "cars number");
		ferris_wheel_cars_number->range(1, 16);
		ferris_wheel_cars_number->value(6);
		ferris_wheel_cars_number->type(FL_HORIZONTAL);
		pty += 30;
		ferris_wheel_speed = new Fl_Value_Slider(ptx, pty, 140, 20, "speed");
		ferris_wheel_speed->range(0.003, 0.3);
		ferris_wheel_speed->value(0.03);
		ferris_wheel_speed->type(FL_HORIZONTAL);
		pty += 30;

		// terrain
		pty = 5; ptx = 500;
		colorBlock = new Fl_Button(ptx, pty, 140, 20, "color block");
		colorBlock->callback((Fl_Callback*)toggle, this);
		pty += 30;
		pencil = new Fl_Button(ptx, pty, 140, 20, "pencil");
		pencil->callback((Fl_Callback*)toggle, this);
		pty += 30;
		debug_mode = new Fl_Button(ptx, pty, 140, 20, "debugMode");
		debug_mode->callback((Fl_Callback*)toggle, this);
		pty += 30;
		camGround = new Fl_Button(ptx, pty, 140, 20, "ground camera");
		camGround->callback((Fl_Callback*)toggle, this);
		pty += 30;
		camFerris = new Fl_Button(ptx, pty, 140, 20, "ferris wheel camera");
		camFerris->callback((Fl_Callback*)toggle, this);
		pty += 30;
		camRoller = new Fl_Button(ptx, pty, 140, 20, "roller coaster camera");
		camRoller->callback((Fl_Callback*)toggle, this);
	}
	end();

	Fl::add_idle((void (*)(void*))cb, this);
}



