#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <tuple>
#include <Fl/Fl_Gl_Window.h>
#include <AL/alut.h>
#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/Mesh.h"
#include "Camera.H"
#include "SinWave.H"
#include "Skybox.H"
#include "HeightMapWave.H"
#include "InteractiveWave.H"
#include "Terrain.H"
#include "FerrisWheel/FerrisWheel.H"
#include "RollerCoaster/RollerCoaster.H"
#include "BillboardObject.h"
#include "Particles.h"
#include "Model.h"

#define PI 3.14159

class Scene : public Fl_Gl_Window
{
public:
	Scene();
	Scene(int _x, int _y, int _w , int _h, const char* l = 0);
	void init();
	void setUp();
	void NPR1();
	virtual int handle(int);
	virtual void draw();
	
	bool loaded = false;
	Camera* camera;
	Skybox* skybox;
	SinWave* sinWave;
	HeightMapWave* heightMap;
	InteractiveWave* interactiveWave;
	Terrain* terrain;
	FerrisWheel* ferrisWheel;
	RollerCoaster* rollerCoaster;
	BillboardObject* sun;
	BillboardObject* signForRollerCoaster;
	BillboardObject* signForFerrisWheel;
	Particles* rain;
	Particles* smoke;
	Model* jackpack;
	Shader* shaderForJackpack;


	FBO* simpleFbo, * edgeFbo, * colorBlockFbo, * mergeFbo;
	unsigned int texture;
	Shader* screenShader = nullptr;
	float quadVertices[24] = { 
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
};

#endif