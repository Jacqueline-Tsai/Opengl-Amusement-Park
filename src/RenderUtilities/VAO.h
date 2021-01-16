#ifndef VAO_H
#define VAO_H

#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
using std::vector;

class VAO{
public:
	VAO() {
		glGenVertexArrays(1, &ID);
		bind();
	}
	~VAO() {
		glDeleteVertexArrays(1, &ID);
	}
	void bind() {
		glBindVertexArray(ID);
	}
	void unbind() {
		glBindVertexArray(0);
	}
	unsigned int ID;
};


#endif