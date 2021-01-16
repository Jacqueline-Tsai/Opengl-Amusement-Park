#ifndef SINWAVE_H
#define SINWAVE_H

#include <vector>
#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/Mesh.h"
#include "Global.H"
using std::vector;

class SinWave {
public:
	SinWave();
	void draw();
	void init();

	bool loaded = false;
	float t = 0;
	VAO* vao;
	VBO* vbo;
	Texture2D* texture = nullptr;
	TextureCubeMap* skytexture = nullptr;
	Texture2D* terrainTex = nullptr;

	Shader* shader = nullptr;
	GLfloat vertices[1800];
	vector<const GLchar*> fileNames;
};

#endif