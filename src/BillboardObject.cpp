#include <iostream>
#include <FileSystem>
#include <fstream>
#include <Fl/fl.h>
#include <windows.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glu.h>
#include "BillboardObject.h"

Shader* BillboardObject::defaultShader = nullptr;
Shader* BillboardObject::sunShader = nullptr;

BillboardObject::BillboardObject(std::string filename, float w, float h, const glm::vec3& centerPosition, bool flipVerticallyOrNot) {

	this->filename = filename;
	this->centerPosition = new float[3];
	this->centerPosition[0] = centerPosition.x;	this->centerPosition[1] = centerPosition.y;	this->centerPosition[2] = centerPosition.z;
	this->width = w;
	this->height = h;
	this->modeForFrag = 1;
	this->flipVerticallyOrNot = flipVerticallyOrNot;
}

BillboardObject::BillboardObject(std::string filename, float w, float h, GLfloat* centerPosition, bool flipVerticallyOrNot) {

	this->filename = filename;
	this->centerPosition = centerPosition;
	this->width = w;
	this->height = h;
	this->modeForFrag = 1;
	this->flipVerticallyOrNot = flipVerticallyOrNot;
}

BillboardObject::BillboardObject(float w, float h, GLfloat* color, GLfloat* centerPosition) {

	this->color = color;
	this->centerPosition = centerPosition;
	this->width = w;
	this->height = h;
	this->modeForFrag = 0;
	this->flipVerticallyOrNot = false;
}

BillboardObject::~BillboardObject() {

	if (vao != nullptr) {

		delete(vao);
	}

	if (vbo != nullptr) {

		delete(vbo);
	}

	if (ebo != nullptr) {

		delete(ebo);
	}

	if (texture != nullptr) {

		delete(texture);
	}
}

void BillboardObject::init() {

	if (modeForFrag == 1) {

		this->texture = new Texture2D(filename.c_str(), this->flipVerticallyOrNot);
	}
	
	vertices = new GLfloat[5 * 4];
	for (int i = 0; i < 4; i++) {

		vertices[i * 5 + 0] = this->width / 2 * this->verticesMultiplier[i * 3 + 0];
		vertices[i * 5 + 1] = this->height / 2 * this->verticesMultiplier[i * 3 + 1];
		vertices[i * 5 + 2] = 0.0f;
		vertices[i * 5 + 3] = this->texCoords[i * 2 + 0];
		vertices[i * 5 + 4] = this->texCoords[i * 2 + 1];
		//std::cout << vertices[i * 5 + 0] << " / " << vertices[i * 5 + 1] << " / " << vertices[i * 5 + 2] << " / " << vertices[i * 5 + 3] << " / " << vertices[i * 5 + 4] << std::endl;
	}

	vao = new VAO();
	vbo = new VBO();
	// No need to attach vertices now, since it will change everytime. 
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 30, vertices, GL_STATIC_DRAW);     
	ebo = new EBO();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    vao->unbind();
}

void BillboardObject::draw(Shader* shader) {
	glDepthMask(GL_FALSE);
	if (defaultShader == nullptr) {

		defaultShader = new Shader("../src/shaders/billboardObject.vert", nullptr, nullptr, nullptr, "../src/shaders/billboardObject.frag");
	}
	if (sunShader == nullptr) {

		sunShader = new Shader("../src/shaders/billboardObjectSkyboxVersion.vert", nullptr, nullptr, nullptr, "../src/shaders/billboardObject.frag");
	}

	if (loaded == false) {

		loaded = true;
		init();
	}

	this->shader = shader;
	if (this->shader == nullptr) {

		this->shader = defaultShader;
	}
	this->shader->Use();

    GLfloat view[16]; 
	GLfloat projection[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
	glUniformMatrix4fv(glGetUniformLocation(this->shader->ID, "view"), 1, GL_FALSE, view);
	glUniformMatrix4fv(glGetUniformLocation(this->shader->ID, "projection"), 1, GL_FALSE, projection);
	if (modeForFrag == 1) {

		glUniform1i(glGetUniformLocation(this->shader->ID, "billboardTexture"), 0);
	}
	else {

		glUniform4fv(glGetUniformLocation(this->shader->ID, "billboardColor"), 1, color);
	}
	glUniform1i(glGetUniformLocation(this->shader->ID, "mode"), modeForFrag);

	vao->bind();
	vbo->bind();
	// Generate new vertices by camera's position and direction. 
	GLfloat* newVertices = new GLfloat[20];
	glm::mat4 viewMatrix;
	for (int i = 0; i < 4; i++) {
		
		for (int j = 0; j < 4; j++) {

			viewMatrix[i][j] = view[i * 4 + j];
		}
	}
	glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
	glm::vec3 CameraRight_worldspace = { inverseViewMatrix[0][0], inverseViewMatrix[0][1], inverseViewMatrix[0][2] };
	glm::vec3 CameraUp_worldspace = { inverseViewMatrix[1][0], inverseViewMatrix[1][1], inverseViewMatrix[1][2] };
	//std::cout << CameraRight_worldspace.x << " / " << CameraRight_worldspace.y << " / "  << CameraRight_worldspace.z << " / "  << CameraUp_worldspace.x << " / " << CameraUp_worldspace.y << " / " << CameraUp_worldspace.z << std::endl;
	for (int i = 0; i < 4; i++) {

		newVertices[i * 5 + 0] = centerPosition[0] + vertices[i * 5 + 0] * CameraRight_worldspace.x + vertices[i * 5 + 1] * CameraUp_worldspace.x;
		newVertices[i * 5 + 1] = centerPosition[1] + vertices[i * 5 + 0] * CameraRight_worldspace.y + vertices[i * 5 + 1] * CameraUp_worldspace.y;
		newVertices[i * 5 + 2] = centerPosition[2] + vertices[i * 5 + 0] * CameraRight_worldspace.z + vertices[i * 5 + 1] * CameraUp_worldspace.z;
		newVertices[i * 5 + 3] = vertices[i * 5 + 3];
		newVertices[i * 5 + 4] = vertices[i * 5 + 4];
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 20, newVertices, GL_STATIC_DRAW);     
	texture->bind(0);
	//glDrawArrays(GL_TRIANGLES, 0, 2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	vao->unbind();
	delete(newVertices);
	glDepthMask(GL_TRUE);
}
						
