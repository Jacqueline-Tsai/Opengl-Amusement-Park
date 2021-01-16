#pragma once
#include <vector>
#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/Mesh.h"
#include "Global.H"
using std::vector;

class BillboardObject {

public:
	BillboardObject(std::string filename, float w, float h, const glm::vec3& centerPosition, bool flipVerticallyOrNot = false);
	BillboardObject(std::string fileName, float w, float h, GLfloat* centerPosition, bool flipVerticallyOrNot = false);
	BillboardObject(float w, float h, GLfloat* color, GLfloat* centerPosition);
	~BillboardObject();
	void init();
	void draw(Shader* shader = nullptr);

	int modeForFrag;
	bool loaded = false;
	VAO* vao = nullptr;
	VBO* vbo = nullptr;
	EBO* ebo = nullptr;
	std::string filename;
	GLfloat* centerPosition;
	float width, height;
	Texture2D* texture = nullptr;
	Shader* shader = nullptr;
	static Shader* defaultShader;
	static Shader* sunShader;
	GLfloat* vertices = nullptr;
	GLfloat* color = nullptr;
	bool flipVerticallyOrNot;

	GLfloat verticesMultiplier[3 * 4] = { -1.0f, -1.0f, 0.0f,
											-1.0f,  1.0f, 0.0f,
											 1.0f,  1.0f, 0.0f,
											 1.0f, -1.0f, 0.0f, };

	GLfloat texCoords[2 * 4] = { 0.0f, 0.0f,
								0.0f, 1.0f,
								1.0f, 1.0f,
								1.0f, 0.0f, };

	unsigned int indices[6] = {
		0, 1, 2,
		0, 2, 3,
	};
};
