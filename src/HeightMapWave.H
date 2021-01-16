#ifndef HEIGHTMAPWAVE_H
#define HEIGHTMAPWAVE_H

#include <vector>
#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/Mesh.h"
#include "Global.H"
using std::vector;

class HeightMapWave {
public:
	HeightMapWave();
	void draw();
	void init();
	void pick(int x, int y);

	bool loaded = false;
	int mapIndex = 0;
	VAO* vao;
	VBO* vbo;//, FBO;
	Texture2D* surfaceTex = nullptr;
	TextureCubeMap* skyTex = nullptr;
	Texture2D* heightMaps[198];
	Texture2D* terrainTex = nullptr;

	Shader* shader = nullptr;
	GLfloat vertices[4718592];
	vector<const GLchar*> fileNames;
};

#endif