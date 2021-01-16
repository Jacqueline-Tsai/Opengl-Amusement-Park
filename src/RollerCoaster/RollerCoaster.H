#ifndef ROLLERCOASTER_H
#define ROLLERCOASTER_H

#include <vector>
#include <map>
#include "Car.H"
#include "Rail.H"
class Car;
using std::vector;

class RollerCoaster {
public:
	RollerCoaster();
	void draw();

	GLuint VAO, VBO, EBO;
	Shader* shader = nullptr;
	glm::vec3 ctrlPnts[16];
	float rotates[3200];
	Vertex lineSeg[2][3200];
	glm::vec3 position[3200];
	glm::vec3 xyzAxis[3200][3];
	float lengths[3200];
	vector<Rail*> rails;
	Car car;
	int pntsNum = 16, carsNum = 5, railsNum, lineSegNum = 200, infoNum = 3200;
	float baseSpd = 0.5, speed = 0.2 , railInterval = 0.02, carInterval = 0.06, totalLen = 0, carPos = 0;

};

#endif