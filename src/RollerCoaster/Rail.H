#ifndef RAIL_H
#define RAIL_H

#include <vector>
#include "../Global.H"
#include "../RenderUtilities/BufferObject.h"
#include "../RenderUtilities/Shader.h"
#include "../RenderUtilities/Texture.h"
#include "../RenderUtilities/Mesh.h"

using std::vector;

class Rail {
public:
	Rail(const glm::mat4& _model_matrix, glm::vec3 xyzAxis[3], const glm::vec3& pos);
	void draw();

	GLuint VAO, VBO, EBO;
	Shader* shader = nullptr;
	float t = 0, speed;
	Triangle rail[12];
	glm::mat4 model_matrix;
};

#endif