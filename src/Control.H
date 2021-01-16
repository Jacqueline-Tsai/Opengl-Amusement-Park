#ifndef CONTROL_H
#define CONTROL_H

#include <vector>
#include <tuple>
#include <Fl/Fl_Gl_Window.h>
#include <Fl/Fl_Double_Window.h>
#include <Fl/Fl_Button.h>
#include <Fl/Fl_Group.H>
#include <Fl/Fl_Value_Slider.H>
#include <Fl/Fl_Browser.H>
#include <FL/Fl_Box.h>
#include <AL/alut.h>
#include "Global.H"


class Control : public Fl_Window {
public:
	Control();
	Control(int x, int y, int , int h, const char* l = 0);

public:

public:
	// sinwave
	Fl_Value_Slider* height;
	Fl_Value_Slider* amplitude;
	Fl_Value_Slider* waveLen;
	Fl_Value_Slider* waveSpd;

	// terrain
	Fl_Value_Slider* offset_max;
	Fl_Value_Slider* d_offset;
	Fl_Value_Slider* terrain_lv;

	// Ferris Wheel
	Fl_Value_Slider* ferris_wheel_speed;
	Fl_Value_Slider* ferris_wheel_cars_number;

	Fl_Button* colorBlock;
	Fl_Button* pencil;
	Fl_Button* debug_mode;
	Fl_Button* camGround;
	Fl_Button* camFerris;
	Fl_Button* camRoller;
};

#endif