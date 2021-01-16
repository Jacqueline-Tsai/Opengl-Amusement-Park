#ifndef PILLAR_H
#define PILLAR_H

#include <vector>
#include "../Global.H"
#include "../RenderUtilities/BufferObject.h"
#include "../RenderUtilities/Shader.h"
#include "../RenderUtilities/Texture.h"
#include "../RenderUtilities/Mesh.h"
#include "Cabin.H"
using std::vector;

class Pillar {
public:
	Pillar(int _lv = 1, bool _camera = false);
	void draw(const glm::mat4& model_matrix);

	GLuint VAO, VBO, EBO;
	Shader* shader = nullptr;
	float t = 0, speed;
	clock_t time;
	int lv;	bool camera;
	Vertex support[6];
	Cabin* cabin = nullptr;
	vector<Pillar*> pillar;
};

#endif