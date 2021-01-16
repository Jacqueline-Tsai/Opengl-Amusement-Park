#ifndef FERRISWHEEL_H
#define FERRISWHEEL_H

#include <vector>
#include "Pillar.H"
using std::vector;

class FerrisWheel {
public:
	FerrisWheel();
	void draw();

	vector<Pillar*> pillar;
	float t = 0;
	
	GLuint VAO, VBO;
	Shader* shader = nullptr;

	//Triangle support[200];
	Vertex support[10];
};

#endif