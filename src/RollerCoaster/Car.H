#ifndef CAR_H
#define CAR_H

#include <vector>
#include "../Global.H"
#include "../RenderUtilities/BufferObject.h"
#include "../RenderUtilities/Shader.h"
#include "../RenderUtilities/Texture.h"
#include "../RenderUtilities/Mesh.h"
using std::vector;

class Car {
public:
	Car();
	void draw(glm::mat4& model_matrix, glm::vec3 xyzAxis[3], const glm::vec3& pos);
	
	GLuint VAO, VBO;
	Texture2D* texture = nullptr;
	Shader* shader = nullptr;
	Triangle car[12];
};

#endif