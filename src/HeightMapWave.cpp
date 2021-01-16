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
#include "HeightMapWave.H"
using std::string;

HeightMapWave::HeightMapWave(){
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

void HeightMapWave::init() {
	shader = new Shader("../src/shaders/heightMapWave.vert", nullptr, nullptr, nullptr, "../src/shaders/heightMapWave.frag");
    surfaceTex = new Texture2D("../Images/water.png");
    skyTex = new TextureCubeMap(fileNames);
    terrainTex = new Texture2D("../Images/terrain/texture.bmp");

    string fileName = "../Images/heightmap/000.png";
    for (int i = 0;i < 198;i++) {
        heightMaps[i] = new Texture2D(fileName.c_str());
        fileName[22] += 1;
        if (fileName[22] > '9') {
            fileName[22] = '0';
            fileName[21] += 1;
        }
        if (fileName[21] > '9') {
            fileName[21] = '0';
            fileName[20] += 1;
        }
    }

    vao = new VAO();
    vbo = new VBO();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    vao->unbind();
}

void HeightMapWave::draw() {
    if (!loaded) {
        init();loaded = true;
    }else {
        mapIndex++;
        if (mapIndex > 197)mapIndex = 0;
    }
    shader->Use();
    GLfloat view[16], projection[16];
    glm::mat4 model;  glm::vec3 cam;
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);    
    cam.x = -(view[0] * view[12] + view[1] * view[13] + view[2] * view[14]);
    cam.y = -(view[4] * view[12] + view[5] * view[13] + view[6] * view[14]);
    cam.z = -(view[8] * view[12] + view[9] * view[13] + view[10] * view[14]);
    model = glm::translate(model, cam);
    model = glm::scale(model, glm::vec3(10, 10, 10));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shader->ID, "cameraPos"), cam.x, cam.y, cam.z);
    glUniform1i(glGetUniformLocation(shader->ID, "skybox"), 0);
    glUniform1i(glGetUniformLocation(shader->ID, "heightmap"), 1);
    
    vao->bind();
    skyTex->bind(0);
    heightMaps[mapIndex]->bind(1);
    glDrawArrays(GL_TRIANGLES, 0, 1572864);
    vao->unbind();
}