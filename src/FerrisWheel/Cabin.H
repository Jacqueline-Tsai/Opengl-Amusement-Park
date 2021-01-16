#ifndef CABIN_H
#define CABIN_H

#include <vector>
#include "../Global.H"
#include "../RenderUtilities/BufferObject.h"
#include "../RenderUtilities/Shader.h"
#include "../RenderUtilities/Texture.h"
#include "../RenderUtilities/Mesh.h"
using std::vector;

class Cabin {
public:
	Cabin();
	void draw(glm::mat4& model_matrix, bool camera = false);

	GLuint VAO, VBO, EBO, textureID;
	Shader* shader = nullptr;
	Triangle box[12];
};

#endif