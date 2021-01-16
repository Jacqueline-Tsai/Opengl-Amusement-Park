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
#include "SinWave.H"

SinWave::SinWave(){
    fileNames.resize(6);
    fileNames[0] = "../Images/skybox/right.jpg";
    fileNames[1] = "../Images/skybox/left.jpg";
    fileNames[2] = "../Images/skybox/top.jpg";
    fileNames[3] = "../Images/skybox/bottom.jpg";
    fileNames[4] = "../Images/skybox/back.jpg";
    fileNames[5] = "../Images/skybox/front.jpg";

    for (int i = 0;i < 1800;i++)vertices[i] = 0;
    for (int i = 0;i < 100;i++) {
        vertices[i * 18 + 0] = i / 100.0;   vertices[i * 18 + 2] = 0;
        vertices[i * 18 + 3] = i / 100.0;   vertices[i * 18 + 5] = 1;
        vertices[i * 18 + 6] = (1 + i) / 100.0;  vertices[i * 18 + 8] = 0;
        vertices[i * 18 + 9] = i / 100.0;   vertices[i * 18 + 11] = 1;
        vertices[i * 18 + 12] = (1 + i) / 100.0;   vertices[i * 18 + 14] = 0;
        vertices[i * 18 + 15] = (1 + i) / 100.0;  vertices[i * 18 + 17] = 1;
    }
}

void SinWave::init() {
	shader = new Shader("../src/shaders/sinWave.vert", nullptr, nullptr, nullptr, "../src/shaders/sinWave.frag");
    texture = new Texture2D("../Images/water.png");
    skytexture = new TextureCubeMap(fileNames);
    terrainTex = new Texture2D("../Images/terrain/texture.bmp");

    vao = new VAO();
    vbo = new VBO();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    vao->unbind();
}

void SinWave::draw() {
    if (!loaded) {
        init();loaded = true;
    }else {
        t += 0.1;
    }
    shader->Use();
    GLfloat view[16]; GLfloat projection[16];
    glm::mat4 model;
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    glUniform1f(glGetUniformLocation(shader->ID, "amplitude"), Global::amplitude);
    glUniform1f(glGetUniformLocation(shader->ID, "height"), Global::height);
    glUniform1f(glGetUniformLocation(shader->ID, "wavelength"), Global::waveLen);
    glUniform1f(glGetUniformLocation(shader->ID, "speed"), Global::speed);
    glUniform1f(glGetUniformLocation(shader->ID, "time"), t);

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shader->ID, "cameraPos"), Global::position.x, Global::position.y, Global::position.z);
    glUniform1i(glGetUniformLocation(shader->ID, "skybox"), 0);
    glUniform1i(glGetUniformLocation(shader->ID, "surface"), 1);
    glUniform1i(glGetUniformLocation(shader->ID, "terrain"), 2);

    vao->bind();
    skytexture->bind(0);
    texture->bind(1);
    terrainTex->bind(2);
    glDrawArrays(GL_TRIANGLES, 0, 600);
    vao->unbind();
    
}