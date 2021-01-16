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
#include "Particles.h"

Shader* Particle::defaultShader = nullptr;
Shader* Particles::defaultShader = nullptr;

Particle::Particle(int threshold, float velocity, float w, float h, GLfloat* color, GLfloat* centerPosition, float startingTime) : BillboardObject(w, h, color, centerPosition) {

	this->threshold = threshold;
	this->velocity = velocity;
	this->startingTime = startingTime;
	this->modeForFrag = 0;
	
	vertices = new GLfloat[5 * 4];
	for (int i = 0; i < 4; i++) {

		vertices[i * 5 + 0] = this->width / 2 * this->verticesMultiplier[i * 3 + 0];
		vertices[i * 5 + 1] = this->height / 2 * this->verticesMultiplier[i * 3 + 1];
		vertices[i * 5 + 2] = 0.0f;
		vertices[i * 5 + 3] = this->texCoords[i * 2 + 0];
		vertices[i * 5 + 4] = this->texCoords[i * 2 + 1];
		//std::cout << vertices[i * 5 + 0] << " / " << vertices[i * 5 + 1] << " / " << vertices[i * 5 + 2] << " / " << vertices[i * 5 + 3] << " / " << vertices[i * 5 + 4] << std::endl;
	}

	//BillboardObject::init();
}

Particle::~Particle() {

	
}

// Depricated
bool Particle::isAlive() {

	// Leave to GPU to calculate the position. 
	if (Global::getCurrentTime() - startingTime >= threshold) {

		startingTime = 0;
		return false;
	}
	return true;
}

void Particle::draw(Shader* shader) {

	std::cerr << "Particle::draw(Shader* shader) has depricated, please use BillboardObject for single object or Particles for multi-object. " << std::endl;
	exit(1);
	/*
	if (defaultShader == nullptr) {

		defaultShader = new Shader("../src/shaders/particles.vert", nullptr, nullptr, nullptr, "../src/shaders/billboardObject.frag");
	}

	this->shader = shader;
	if (this->shader == nullptr) {

		this->shader = defaultShader;
	}
	this->shader->Use();
	glUniform1f(glGetUniformLocation(this->shader->ID, "velocity"), this->velocity);
	glUniform1f(glGetUniformLocation(this->shader->ID, "duration"), Global::getCurrentTime() - this->startingTime);
	BillboardObject::draw(this->shader);
	*/
}

struct Layout* Particle::getLayout() {

	struct Layout* newData = new struct Layout[4];
	float duration = Global::getCurrentTime() - startingTime;
	for (int i = 0; i < 4; i++) {

		newData[i].vertices.x = vertices[i * 5 + 0];
		newData[i].vertices.y = vertices[i * 5 + 1];
		newData[i].vertices.z = vertices[i * 5 + 2];
		newData[i].texCoords.x = vertices[i * 5 + 3];
		newData[i].texCoords.y = vertices[i * 5 + 4];
		newData[i].centerPosition.x = centerPosition[0];
		newData[i].centerPosition.y = centerPosition[1];
		newData[i].centerPosition.z = centerPosition[2];
		newData[i].velocity = velocity;
		newData[i].duration = duration; // send its existence time. 
		//std::cout << newData[i].duration << std::endl;
		//system("pause");
	}
	return newData;
	/*
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
	*/
}

Particles::Particles(float dencity, int minTime, int maxTime, GLfloat* centerPosition, float w, float l, GLfloat* direction, float minVelocity, float maxVelocity, GLfloat* color, float particleW, float particleL, int LODW, int LODL, float maxVisibleDistance) {

	this->dencity = dencity;
	this->minTime = minTime;
	this->maxTime = maxTime;
	this->w = w;
	this->l = l;
	this->centerPosition = centerPosition;
	this->direction = direction;
	this->maxVelocity = maxVelocity;
	this->minVelocity = minVelocity;
	this->color = color;
	this->particleW = particleW;
	this->particleL = particleL;
	this->LODW = LODW;
	this->LODL = LODL;
	this->maxVisibleDistance = maxVisibleDistance;
	init();
}

void Particles::init() {

	int averageNumber = (float)(minTime + maxTime) / 2.0f * dencity * LODW * LODL;
	this->numberOfParticlesInAnIteration = dencity * LODL * LODW;
	this->duration = this->maxTime - this->minTime;
	this->differenceOfVelocity = maxVelocity - minVelocity;
	glm::vec3 normalDirection = glm::normalize(glm::vec3(this->direction[0], this->direction[1], this->direction[2]));
	this->direction[0] = normalDirection.x;
	this->direction[1] = normalDirection.y;
	this->direction[2] = normalDirection.z;
	particles.reserve(averageNumber);
	vao = new VAO();
	vbo = new VBO();
	ebo = new EBO();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Layout), (GLvoid*)offsetof(struct Layout, vertices));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct Layout), (GLvoid*)offsetof(struct Layout, texCoords));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct Layout), (GLvoid*)offsetof(struct Layout, centerPosition));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(struct Layout), (GLvoid*)offsetof(struct Layout, velocity));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(struct Layout), (GLvoid*)offsetof(struct Layout, duration));
	vao->unbind();
}

Particle* Particles::getAParticle() {

	//std::cout << "Getting a Particle..." << std::endl;
	GLfloat* particlePosition = new GLfloat[3];
	float randomForX = (float)rand() / (float)RAND_MAX;
	float randomForZ = (float)rand() / (float)RAND_MAX;
	if (rand() % 2) {

		randomForX *= -1.0f;
	}
	if (rand() % 2) {

		randomForZ *= -1.0f;
	}
	particlePosition[0] = centerPosition[0] + randomForX * w / 2.0f;
	particlePosition[1] = centerPosition[1];
	particlePosition[2] = centerPosition[2] + randomForZ * l / 2.0f;
	return new Particle(
	(float)rand() / (float)RAND_MAX * duration + minTime,
		minVelocity + differenceOfVelocity * (float)rand() / (float)RAND_MAX,
		particleW, particleL, color,
		particlePosition, 
		Global::getCurrentTime());
}

void Particles::update() {

	for (int i = 0; i < particles.size(); i++) {

		if (particles[i]->isAlive() == false) {

			delete(particles[i]->centerPosition);
			delete(particles[i]->vertices);
			delete(particles[i]);
			particles.erase(particles.begin() + i);
		}
	}
	for (int i = 0; i < numberOfParticlesInAnIteration; i++) {

		particles.push_back(getAParticle());
	}
}

float Particles::distance(float firstX, float firstY, float firstZ, float secondX, float secondY, float secondZ) {

	return sqrt(pow(firstX - secondX, 2) + pow(firstY - secondY, 2) + pow(firstZ - secondZ, 2));
}

void Particles::draw(Shader* shader) {

	if (defaultShader == nullptr) {

		defaultShader = new Shader("../src/shaders/particles.vert", nullptr, nullptr, nullptr, "../src/shaders/billboardObject.frag");
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
	glm::mat4 viewMatrix;
	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {

			viewMatrix[i][j] = view[i * 4 + j];
		}
	}
	glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
	GLfloat inverseView[16];
	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {

			inverseView[i * 4 + j] = inverseViewMatrix[i][j];
		}
	}
	glUniformMatrix4fv(glGetUniformLocation(this->shader->ID, "view"), 1, GL_FALSE, view);
	glUniformMatrix4fv(glGetUniformLocation(this->shader->ID, "inverseView"), 1, GL_FALSE, inverseView);
	glUniformMatrix4fv(glGetUniformLocation(this->shader->ID, "projection"), 1, GL_FALSE, projection);
	glUniform4fv(glGetUniformLocation(this->shader->ID, "billboardColor"), 1, color);
	glUniform1i(glGetUniformLocation(this->shader->ID, "mode"), 0);
	glUniform3fv(glGetUniformLocation(this->shader->ID, "direction"), 1, direction);
	glUniform3f(glGetUniformLocation(this->shader->ID, "cameraPosition"), Global::position.x, Global::position.y, Global::position.z);
	glUniform1f(glGetUniformLocation(this->shader->ID, "maxVisibleDistance"), maxVisibleDistance);

	struct Layout* finalLayout = new struct Layout[particles.size() * 4];
	int particleShowingNumber = 0;
	for (int i = 0; i < particles.size(); i++) {

		struct Layout* layout = particles[i]->getLayout();
		//if (i == 0) {

		//	std::cout << layout->time << std::endl;
		//}
		/*
		float distanceOfParticle = distance(
			Global::position.x,
			Global::position.y,
			Global::position.z,
			layout->centerPosition.x,
			layout->centerPosition.y,
			layout->centerPosition.z);

		if (distanceOfParticle > maxVisibleDistance) {

			delete(layout);
			continue;
		}
		else {

			if (float(rand()) / (float)RAND_MAX > distanceOfParticle / maxVisibleDistance) {

				delete(layout);
				continue;
			}
		}
		*/
		particleShowingNumber++;
		for (int j = 0; j < 4; j++) {

			finalLayout[i * 4 + j] = layout[j];
		}
		delete(layout);
	}
	//std::cout << particleShowingNumber << std::endl;

	unsigned int basicIndices[6] = {
		0, 1, 2,
		0, 2, 3,
	};

	unsigned int* indices = new unsigned int[6 * particleShowingNumber];
	for (int i = 0; i < particleShowingNumber; i++) {

		for (int j = 0; j < 6; j++) {

			indices[i * 6 + j] = basicIndices[j] + 4 * i;
		}
	}

	vao->bind();
	vbo->bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct Layout) * particleShowingNumber * 4, finalLayout, GL_DYNAMIC_DRAW);
	ebo->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * particleShowingNumber * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLES, 6 * particles.size(), GL_UNSIGNED_INT, 0);
	vao->unbind();
	delete(finalLayout);
	delete(indices);
	// If the memory usage keeps rising up, it might because this Paricle class. 
}

