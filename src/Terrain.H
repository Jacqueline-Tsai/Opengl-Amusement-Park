#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <unordered_map>
#include "Global.H"
#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/Mesh.h"
using std::vector;

class Terrain {
public:
	Terrain();
	void draw();
	void genHeight();
	float genRand(int lv);


	GLuint VAO, VBO;
	Texture2D* texture = nullptr;
	Texture2D* heightMap = nullptr;

	Shader* shader = nullptr;
	GLfloat vertices[73728];//2*4^n*3*3
	vector<Triangle*> terrain;
	Triangle terrain0[18];
	Triangle terrain1[72];
	Triangle terrain2[288];
	Triangle terrain3[1152];
	Triangle terrain4[4608];
	Triangle terrain5[18432];
	Triangle terrain6[73728];
	int maxLv = 6, size = 73728;//18*4^n
};

#endif