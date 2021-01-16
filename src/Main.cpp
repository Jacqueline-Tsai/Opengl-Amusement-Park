#include "Scene.H"
#include "Control.H"
#include "Global.H"
#include <Fl/Fl.h>
#include <cstdlib>
#include <ctime>

int main(int, char**)
{
	srand(time(NULL));
	Scene scene(5, 5, 1200, 600);
	Control control(5, 5, 1200, 600, "control");
	Global::control = &control;
	scene.show();
	control.show();
	Fl::run();
}