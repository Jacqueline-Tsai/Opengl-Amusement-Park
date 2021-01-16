#pragma once
#include <vector>
#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/Mesh.h"
#include "Global.H"
#include "BillboardObject.h"
using std::vector;

struct Layout {

	glm::vec3 vertices;
	glm::vec2 texCoords;
	glm::vec3 centerPosition;
	GLfloat velocity;
	GLfloat duration;
};

class Particle : public BillboardObject {

public:

	Particle(int threshold, float velocity, float w, float h, GLfloat* color, GLfloat* centerPosition, float startingTime);
	~Particle();
	bool isAlive();
	struct Layout* getLayout();
	void draw(Shader* shader = nullptr);

	int threshold;
	float velocity;
	float startingTime;
	static Shader* defaultShader;
};

class Particles {

public:

	Particles(float dencity, int minTime, int maxTime, GLfloat* centerPosition, float w, float l, GLfloat* direction, float minVelocity, float maxVelocity, GLfloat* color, float particleW, float particleL, int LODW, int LODL, float maxVisibleDistance);
	void update();
	void init();
	Particle* getAParticle();
	void reGenerateAParticle(Particle* particle); // May improve performance, but haven't write this. 
	float distance(float firstX, float firstY, float firstZ, float secondX, float secondY, float Z);
	void draw(Shader* shader = nullptr);

	VAO* vao = nullptr;
	VBO* vbo = nullptr;
	EBO* ebo = nullptr;
	Shader* shader = nullptr;
	float dencity;
	int numberOfParticlesInAnIteration;
	int minTime, maxTime;
	int duration;
	GLfloat* centerPosition;
	float w, l;
	GLfloat* direction;
	float maxVelocity, minVelocity;
	float differenceOfVelocity;
	GLfloat* color;
	float particleW, particleL;
	int LODW, LODL;
	float maxVisibleDistance;
	static Shader* defaultShader;
	vector<Particle*> particles;
	unsigned int* indices;
};
