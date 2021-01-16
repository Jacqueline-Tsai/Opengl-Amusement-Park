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
#include <filesystem>
#include "Scene.H"

static unsigned long lastdraw = 0;
void cb(Scene* scene) {
	if (clock() - lastdraw > CLOCKS_PER_SEC / 30) {
		lastdraw = clock();
		scene->damage(1);
	}
}

Scene::Scene() : Scene(5, 5, 590, 590, 0)
{
	
}
Scene::Scene(int _x, int _y, int _w, int _h, const char* l) : Fl_Gl_Window(_x, _y, _w, _h, l)
{
	mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);
	camera = new Camera(w(), h());
	
	Fl::add_idle((void (*)(void*))cb, this);
}

void Scene::draw()
{
	gladLoadGL();

    camera->use();
	if (!loaded) {
		init();		loaded = true;
	}
	setUp();
	skybox->draw();
	sun->draw(BillboardObject::sunShader);
	rollerCoaster->draw();
	ferrisWheel->draw();
	terrain->draw();
	//heightMap->draw();
	//interactiveWave->draw();
	//sinWave->draw();
	rain->update();
	rain->draw();
	smoke->update();
	smoke->draw();

	shaderForJackpack->Use();
	GLfloat viewArray[16];
	GLfloat projectionArray[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionArray);
	glGetFloatv(GL_MODELVIEW_MATRIX, viewArray);

	glm::vec4* tempVec4 = new glm::vec4[4];
	for (int i = 0; i < 4; i++) {

		tempVec4[i].x = projectionArray[i * 4 + 0];
		tempVec4[i].y = projectionArray[i * 4 + 1];
		tempVec4[i].z = projectionArray[i * 4 + 2];
		tempVec4[i].w = projectionArray[i * 4 + 3];
	}
	glm::mat4 projection = glm::mat4(tempVec4[0], tempVec4[1], tempVec4[2], tempVec4[3]);
	for (int i = 0; i < 4; i++) {

		tempVec4[i].x = viewArray[i * 4 + 0];
		tempVec4[i].y = viewArray[i * 4 + 1];
		tempVec4[i].z = viewArray[i * 4 + 2];
		tempVec4[i].w = viewArray[i * 4 + 3];
	}
	glm::mat4 view = glm::mat4(tempVec4[0], tempVec4[1], tempVec4[2], tempVec4[3]);
	glUniformMatrix4fv(glGetUniformLocation(shaderForJackpack->ID, "projection"),
		1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderForJackpack->ID, "view"),
		1, GL_FALSE, glm::value_ptr(view));
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f)); // 适当调整位置
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // 适当缩小模型
	glUniformMatrix4fv(glGetUniformLocation(shaderForJackpack->ID, "model"),
		1, GL_FALSE, glm::value_ptr(model));
	jackpack->draw(shaderForJackpack);

	signForRollerCoaster->draw();
	signForFerrisWheel->draw();
	
	NPR1();
	
	//interactiveWave->update();
}

int Scene::handle(int event) {
	camera->handle(event);
    interactiveWave->handle(event);
	damage(1);
	return 1;
}

void Scene::init() {
	skybox = new Skybox();
	terrain = new Terrain();
	//sinWave = new SinWave();
	//heightMap = new HeightMapWave();
	//interactiveWave = new InteractiveWave();
	ferrisWheel = new FerrisWheel();
	rollerCoaster = new RollerCoaster();

	sun = new BillboardObject("../Images/sun.png", 30, 30, glm::vec3(-50, 55, -80));
	GLfloat* rainCenterPosition = new GLfloat[3];
	rainCenterPosition[0] = 0.0f;
	rainCenterPosition[1] = 50.0f;
	rainCenterPosition[2] = 0.0f;
	GLfloat* rainColor = new GLfloat[4];
	rainColor[0] = 0.058f;
	rainColor[1] = 0.545f;
	rainColor[2] = 0.933f;
	rainColor[3] = 0.8f;
	GLfloat* direction = new GLfloat[3];
	direction[0] = 0.0f;
	direction[1] = -1.0f;
	direction[2] = 0.0f;
	rain = new Particles(0.03f, 10, 10, rainCenterPosition, 200, 200, direction, 7.5f, 7.5f, rainColor, 0.05f, 0.1f, 100, 100, 70);
	smoke = new Particles(0.5f, 3, 3, &(new glm::vec3(0.0f, 0.0f, 0.0f))->operator[](0), 3, 3,
		&(new glm::vec3(0.0f, 1.0f, 0.0f))->operator[](0), 0.5, 0.5, &(new glm::vec4(1.0f, 1.0f, 1.0f, 0.5))->operator[](0),
		0.02, 0.02, 30, 30, 100);

	jackpack = new Model;
	if (!jackpack->loadModel("../Images/3D_model/Center_Desk/Center_Desk/CLASSIC_COLUMN.obj")) {
		std::cerr << "Object loading failure. " << std::endl;
		std::system("pause");
		exit(1);
	}
	shaderForJackpack = new Shader("../src/shaders/model.vert", nullptr, nullptr, nullptr, "../src/shaders/model.frag");

	signForRollerCoaster = new BillboardObject("../Images/billboard/roller.jpg", 30, 20, Global::roller_coaster_model_matrix * glm::vec4(0, 0.5, 0, 1), true);
	signForFerrisWheel = new BillboardObject("../Images/billboard/ferris.jpg", 30, 20, Global::ferris_wheel_model_matrix * glm::vec4(0, 0.5, 0, 1), true);

	simpleFbo = new FBO(w(), h()), edgeFbo = new FBO(w(), h()), colorBlockFbo = new FBO(w(), h()), mergeFbo = new FBO(w(), h());
	simpleFbo->shader = new Shader("../src/shaders/render/simple.vert", nullptr, nullptr, nullptr, "../src/shaders/render/simple.frag");
	edgeFbo->shader = new Shader("../src/shaders/render/edge.vert", nullptr, nullptr, nullptr, "../src/shaders/render/edge.frag");
	colorBlockFbo->shader = new Shader("../src/shaders/render/colorBlock.vert", nullptr, nullptr, nullptr, "../src/shaders/render/colorBlock.frag");
	mergeFbo->shader = new Shader("../src/shaders/render/merge.vert", nullptr, nullptr, nullptr, "../src/shaders/render/merge.frag");
}

void Scene::setUp() {
	glViewport(0, 0, w(), h());
	glClearColor(0, 0, .3f, 0);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(0);
	simpleFbo->bind();
	Global::update();
}
void Scene::NPR1() {
	/*
	int x = fbo->texture->size.x, y = fbo->texture->size.y;
	//GLuint* pixels = new GLuint[x * 4 * y];
	GLubyte* pixel = new GLubyte[x * 4 * y];
	float* data = new float[x * 4 * y];
	fbo->texture->bind(0);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	for (int i = 0; i < x * y * 4; i++)data[i] = (int)pixel[i] / 255.0f;
	clock_t tmp = clock();
	float min = 1, max = 0;
	float* brightness = (float*)malloc(sizeof(float) * x * y);
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			brightness[i * x + j] = 0.3 * data[(i * x + j) * 4] + 0.59 * data[(i * x + j) * 4 + 1] + 0.11 * data[(i * x + j) * 4 + 2];
			min = fmin(min, brightness[i * x + j]);	max = fmax(max, brightness[i * x + j]);
		}
	}
	float a = 1 / (max - min), b = min * a;
	//for (int i = 0; i < y; i++) {
	//	for (int j = 0; j < x; j++) {
	//		float ratio = ((brightness[i * x + j]) * a - b) / brightness[i * x + j];
	//		for (int k = 0; k < 3; k++) {
	//			data[(i * x + j) * 4 + k] *= ratio;
	//		}
	//	}
	//}
	//float* highpass = new float[x * 4 * y];
	//filt(data, highpass, y, x, { {-1,-1,-1},{-1,8,-1},{-1,-1,-1} }, 9);


	for (int i = 0; i < x * y * 4; i++) {
		pixel[i] = fmax(0, (int)(data[i] * 255));
	}
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	*/
	simpleFbo->unbind();
	if (Global::pencil && Global::colorBlock) {
		edgeFbo->bind();
		edgeFbo->shader->Use();
		glUniform1i(glGetUniformLocation(edgeFbo->shader->ID, "Texture"), 0);
		simpleFbo->draw();
		edgeFbo->unbind();

		colorBlockFbo->bind();
		colorBlockFbo->shader->Use();
		glUniform1i(glGetUniformLocation(colorBlockFbo->shader->ID, "Texture"), 0);
		simpleFbo->draw();
		colorBlockFbo->unbind();

		mergeFbo->shader->Use();
		glUniform1i(glGetUniformLocation(mergeFbo->shader->ID, "edgeTexture"), 0);
		glUniform1i(glGetUniformLocation(mergeFbo->shader->ID, "colorBlockTexture"), 1);
		edgeFbo->vao->bind();
		edgeFbo->texture->bind(0);
		colorBlockFbo->texture->bind(1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		edgeFbo->vao->unbind();
	}
	else {
		if (Global::pencil)  edgeFbo->shader->Use();
		else if (Global::colorBlock) colorBlockFbo->shader->Use();
		else simpleFbo->shader->Use();
		glUniform1i(glGetUniformLocation(colorBlockFbo->shader->ID, "Texture"), 0);
		simpleFbo->draw();
	}

	
}


