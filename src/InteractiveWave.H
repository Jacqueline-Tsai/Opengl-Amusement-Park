#ifndef INTERACTIVEWAVE_H
#define INTERACTIVEWAVE_H

#include <vector>
#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/Mesh.h"
#include "Global.H"
#include "Box.H"
using std::vector;

class InteractiveWave {
public:
	InteractiveWave();
	void draw();
	void init();
	void handle(int event);
	void pick(int x, int y);
	void update();
	void drop();

	bool loaded = false, loaded2 = false;
	int mapIndex = 0;
    VAO* vao, * vaoSide;
	VBO* vbo, * vboSide;// heightFBO, heightTex;
	FBO* fbo;

	Box* tileBox = nullptr;
	TextureCubeMap* skyTex = nullptr;	
    Shader* shader = nullptr, * dropShader = nullptr, * updateShader = nullptr, * sideShader = nullptr;
	GLfloat vertices[4718592];
	GLfloat sideVert[180] = {
        -50.0f,  40.0f, -50.0f, 0.0f,  0.0f, -1.0f,
        -50.0f, -50.0f, -50.0f, 0.0f,  0.0f, -1.0f,
         50.0f, -50.0f, -50.0f, 0.0f,  0.0f, -1.0f,
         50.0f, -50.0f, -50.0f, 0.0f,  0.0f, -1.0f,
         50.0f,  40.0f, -50.0f, 0.0f,  0.0f, -1.0f,
        -50.0f,  40.0f, -50.0f, 0.0f,  0.0f, -1.0f,

        -50.0f, -50.0f,  50.0f, -1.0f, 0.0f,  0.0f,
        -50.0f, -50.0f, -50.0f, -1.0f, 0.0f,  0.0f,
        -50.0f,  40.0f, -50.0f, -1.0f, 0.0f,  0.0f,
        -50.0f,  40.0f, -50.0f, -1.0f, 0.0f,  0.0f,
        -50.0f,  40.0f,  50.0f, -1.0f, 0.0f,  0.0f,
        -50.0f, -50.0f,  50.0f, -1.0f, 0.0f,  0.0f,

         50.0f, -50.0f, -50.0f, 1.0f, 0.0f,  0.0f,
         50.0f, -50.0f,  50.0f, 1.0f, 0.0f,  0.0f,
         50.0f,  40.0f,  50.0f, 1.0f, 0.0f,  0.0f,
         50.0f,  40.0f,  50.0f, 1.0f, 0.0f,  0.0f,
         50.0f,  40.0f, -50.0f, 1.0f, 0.0f,  0.0f,
         50.0f, -50.0f, -50.0f, 1.0f, 0.0f,  0.0f,

        -50.0f, -50.0f,  50.0f, 0.0f,  0.0f, 1.0f,
        -50.0f,  40.0f,  50.0f, 0.0f,  0.0f, 1.0f,
         50.0f,  40.0f,  50.0f, 0.0f,  0.0f, 1.0f,
         50.0f,  40.0f,  50.0f, 0.0f,  0.0f, 1.0f,
         50.0f, -50.0f,  50.0f, 0.0f,  0.0f, 1.0f,
        -50.0f, -50.0f,  50.0f, 0.0f,  0.0f, 1.0f,

        -50.0f, -50.0f, -50.0f, 0.0f, -1.0f, 0.0f,
        -50.0f, -50.0f,  50.0f, 0.0f, -1.0f, 0.0f,
         50.0f, -50.0f, -50.0f, 0.0f, -1.0f, 0.0f,
         50.0f, -50.0f, -50.0f, 0.0f, -1.0f, 0.0f,
        -50.0f, -50.0f,  50.0f, 0.0f, -1.0f, 0.0f,
         50.0f, -50.0f,  50.0f, 0.0f, -1.0f, 0.0f
	};
	GLfloat w = 512, h = 512, ww = 1200, wh = 600;
	vector<const GLchar*> fileNames;

	// test
	void test();
	void test1();
	GLuint Buffer;
	GLuint Texture;



    void init2();
    void draw2();
};

#endif