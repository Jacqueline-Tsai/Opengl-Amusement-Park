#pragma once
#include <glad\glad.h>
#include "Texture.h"
#include "Shader.h"

#define MAX_FBO_TEXTURE_AMOUNT 4
#define MAX_VAO_VBO_AMOUNT 3

class VAO {
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

class EBO {
public:
	EBO() {
		glGenBuffers(1, &ID);
		bind();
	}
	~EBO() {
		glDeleteBuffers(1, &ID);
	}
	void bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}
	void unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	unsigned int ID;
};

class VBO {
public:
	VBO() {
		glGenBuffers(1, &ID);
		bind();
	}
	~VBO() {
		glDeleteBuffers(1, &ID);
	}
	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}
	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	unsigned int ID;
};


class FBO {
public:
	FBO(int w, int h) {
		texture = new Texture2D(w, h);
		this->width = w;
		this->height = h;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		init();
	}
	FBO(const char* path) {
		texture = new Texture2D(path);
		init();
	}
	void init() {
		vao = new VAO();
		vbo = new VBO();
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glGenFramebuffers(1, &ID);
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->ID, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		check();
		unbind();
	}
	~FBO() {
		glDeleteVertexArrays(1, &ID);
	}
	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glClearColor(0, 0, .3f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void draw() {
		vao->bind();
		texture->bind(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		vao->unbind();
	}
	void check()
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Can't initialize an FBO render texture. FBO initialization failed." << std::endl;
	}
	unsigned int ID;
	float quadVertices[24] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	VAO* vao;
	VBO* vbo;
	unsigned int rbo;
	int width, height;
	Texture2D* texture;	//out
	Shader* shader; //in
};


//struct VAO
//{
//	GLuint vao;
//	GLuint vbo[MAX_VAO_VBO_AMOUNT];
//	GLuint ebo;
//	union
//	{
//		unsigned int element_amount;//for draw element
//		unsigned int count;			//for draw array
//	};
//};
//struct UBO
//{
//	GLuint ubo;
//	GLsizeiptr size;
//};
//struct FBO
//{
//	GLuint fbo;	//frame buffer
//	GLuint textures[MAX_FBO_TEXTURE_AMOUNT];	//attach to color buffer
//	GLuint rbo;	//attach to depth and stencil
//};