#include <iostream>
#include <string>
#include <FileSystem>
#include <fstream>
#include <Fl/fl.h>
#include <windows.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glu.h>
#include "InteractiveWave.H"
using std::string;

InteractiveWave::InteractiveWave(){
    fileNames.resize(6);
    fileNames[0] = "../Images/skybox/right.jpg";
    fileNames[1] = "../Images/skybox/left.jpg";
    fileNames[2] = "../Images/skybox/top.jpg";
    fileNames[3] = "../Images/skybox/bottom.jpg";
    fileNames[4] = "../Images/skybox/back.jpg";
    fileNames[5] = "../Images/skybox/front.jpg";

    for (int i = 0;i < 4718592;i++)vertices[i] = 0;
    for (int i = 0;i < 512;i++) {
        for (int j = 0;j < 512;j++) {
            int idx = (i * 512 + j) * 18;
            GLfloat x1 = i / 512.0, x2 = (i + 1) / 512.0,
                z1 = j / 512.0, z2 = (j + 1) / 512.0;
            vertices[idx] = x1;   vertices[idx + 2] = z1;
            vertices[idx + 3] = x1;   vertices[idx + 5] = z2;
            vertices[idx + 6] = x2;   vertices[idx + 8] = z1;
            vertices[idx + 9] = x1;   vertices[idx + 11] = z2;
            vertices[idx + 12] = x2;   vertices[idx + 14] = z1;
            vertices[idx + 15] = x2;  vertices[idx + 17] = z2;
        }
    }
}

void InteractiveWave::init() {
    
	shader = new Shader("../src/shaders/interactiveWave.vert", nullptr, nullptr, nullptr, "../src/shaders/interactiveWave.frag");
    dropShader = new Shader("../src/shaders/drop.vert", nullptr, nullptr, nullptr, "../src/shaders/drop.frag");
    updateShader = new Shader("../src/shaders/update.vert", nullptr, nullptr, nullptr, "../src/shaders/update.frag");
    skyTex = new TextureCubeMap(fileNames);
    fbo = new FBO(ww, wh);//"../Images/black.png"
    vao = new VAO();
    vbo = new VBO();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    vao->unbind();
    
    tileBox = new Box();
}

void InteractiveWave::init2() {
    sideShader = new Shader("../src/shaders/interactiveWaveSide.vert", nullptr, nullptr, nullptr, "../src/shaders/interactiveWave.frag");
    vaoSide = new VAO();
    vboSide = new VBO();
    glBufferData(GL_ARRAY_BUFFER, sizeof(sideVert), &sideVert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    vaoSide->unbind();
}
void InteractiveWave::draw2() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    if (!loaded2) {
        init2();loaded2 = true;
    }
    sideShader->Use();
    GLfloat view[16];
    GLfloat projection[16];
    glm::mat4 model;
    glm::vec3 cam;
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    model = glm::translate(model, cam);
    model = glm::scale(model, glm::vec3(1, 1, 1));
    glUniformMatrix4fv(glGetUniformLocation(sideShader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(sideShader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(sideShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(sideShader->ID, "cameraPos"), Global::position.x, Global::position.y, Global::position.z);
    glUniform1i(glGetUniformLocation(sideShader->ID, "skybox"), 0);
    glUniform1i(glGetUniformLocation(sideShader->ID, "tile"), 1);

    vaoSide->bind();
    skyTex->bind(0);
    tileBox->texture->bind(1);
    glDrawArrays(GL_TRIANGLES, 0, 30);
    vaoSide->unbind();

    glDisable(GL_CULL_FACE);
}
void InteractiveWave::draw() {
    if (!loaded) {
        init();loaded = true;
    }
    tileBox->draw();
    shader->Use();
    GLfloat view[16];
    GLfloat projection[16];
    glm::mat4 model;
    glm::vec3 cam;
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    model = glm::translate(model, cam);
    model = glm::scale(model, glm::vec3(1, 1, 1));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shader->ID, "cameraPos"), Global::position.x, Global::position.y, Global::position.z);
    glUniform1i(glGetUniformLocation(shader->ID, "skybox"), 0);
    glUniform1i(glGetUniformLocation(shader->ID, "tile"), 1);
    glUniform1i(glGetUniformLocation(shader->ID, "heightmap"), 2);

    vao->bind();
    skyTex->bind(0);
    tileBox->texture->bind(1);
    fbo->texture->bind(2);
    glDrawArrays(GL_TRIANGLES, 0, 1572864);
    vao->unbind();

    draw2();
}
void InteractiveWave::handle(int event) {//, glm::vec3 posotion, glm::vec3 lookAt  
    /*if (event == FL_RELEASE && Fl::event_button() == FL_RIGHT_MOUSE) {
        drop();
        glm::vec3 x = glm::cross(Global::lookAt, glm::vec3(0, 1, 0));
        glm::vec3 y = glm::cross(Global::lookAt, x);
        float fixZ = 10, winLen = 2 * tan(Global::FOV / 2), xLen = (Fl::event_x() / ww - 0.5) * winLen, yLen = (Fl::event_y() / wh - 0.5) * winLen;
        float tmp1 = Fl::event_x(), tmp2 = Fl::event_y();
        glm::vec3 vec = Global::lookAt + x * xLen + y * yLen;
        glm::vec3 pos = Global::position + vec / vec.y * (10 - Global::position.y);
    }*/
}
void InteractiveWave::pick(int x, int y){
    
}
void InteractiveWave::update() {

    fbo->bind();
    updateShader->Use();
    fbo->draw();

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    fbo->texture->bind(0);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, ww, wh, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    fbo->texture->unbind(0);
    fbo->unbind();
}
void InteractiveWave::drop() {

    fbo->bind();
    dropShader->Use();
    glUniform1i(glGetUniformLocation(dropShader->ID, "texture"), 0);
    fbo->draw();
    
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    fbo->texture->bind(0);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, ww, wh, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    fbo->texture->unbind(0);
    fbo->unbind();

    
    
}

void InteractiveWave::test()
{
    glGenBuffers(1, &Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer);
    glBufferData(GL_ARRAY_BUFFER, ww * wh * 3, NULL, GL_STREAM_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void InteractiveWave::test1()
{
    glBindBuffer(GL_PIXEL_PACK_BUFFER, Buffer);
    glReadPixels(0, 0, ww, wh, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, Buffer);
    fbo->texture->bind(0);
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ww, wh, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    fbo->texture->unbind(0);
}