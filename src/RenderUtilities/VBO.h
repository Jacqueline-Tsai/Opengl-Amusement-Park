#ifndef VBO_H
#define VBO_H

#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
using std::vector;

class VBO {
public:
	VBO() {
		glGenVertexArrays(1, &ID);
		bind();
	}
	~VBO() {
		glDeleteVertexArrays(1, &ID);
	}
	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}
	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	unsigned int ID;
};


#endif