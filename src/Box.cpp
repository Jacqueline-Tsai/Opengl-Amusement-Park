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
#include "Box.H"

Box::Box() {
    fileNames.resize(6);
    fileNames[0] = "../Images/tiles.jpg";
    fileNames[1] = "../Images/tiles.jpg";
    fileNames[2] = "../Images/tiles.jpg";
    fileNames[3] = "../Images/tiles.jpg";
    fileNames[4] = "../Images/tiles.jpg";
    fileNames[5] = "../Images/tiles.jpg";
}
void Box::init() {
    shader = new Shader("../src/shaders/box.vert", nullptr, nullptr, nullptr, "../src/shaders/box.frag");
    texture = new TextureCubeMap(fileNames);
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    
}

void Box::draw() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    if (!loaded) {
        init();loaded = true;
    }
    shader->Use();
    GLfloat model_view[16];
    GLfloat projection[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, model_view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, model_view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniform1i(glGetUniformLocation(shader->ID, "skybox"), 0);

    glBindVertexArray(VAO);
    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 30);
    glBindVertexArray(0);

    glDisable(GL_CULL_FACE);
}


//
//reflectCube::reflectCube() {
//    faces.resize(6);
//    for (int i = 0;i < 6;i++) {
//        faces[i]= "../Images/blue.png";
//    }
//    faces[0] = "../Images/skybox/right.jpg";
//    faces[1] = "../Images/skybox/left.jpg";
//    faces[2] = "../Images/skybox/top.jpg";
//    faces[3] = "../Images/skybox/bottom.jpg";
//    faces[4] = "../Images/skybox/back.jpg";
//    faces[5] = "../Images/skybox/front.jpg";
//}
//void reflectCube::init() {
//    shader = new Shader("../src/shaders/box.vert", nullptr, nullptr, nullptr, "../src/shaders/box.frag");
//
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//
//    glBindVertexArray(0);
//
//    cubemapTexture = loadCubemap(faces);
//}
//
//
//void reflectCube::draw() {
//    if (!loaded) {
//        init();loaded = true;
//    }
//    shader->Use();
//    GLfloat view[16];
//    GLfloat projection[16];
//    glm::mat4 model;
//    glm::vec3 cam;
//    glGetFloatv(GL_PROJECTION_MATRIX, projection);
//    glGetFloatv(GL_MODELVIEW_MATRIX, view);
//    cam.x = -(view[0] * view[12] + view[1] * view[13] + view[2] * view[14]);
//    cam.y = -(view[4] * view[12] + view[5] * view[13] + view[6] * view[14]);
//    cam.z = -(view[8] * view[12] + view[9] * view[13] + view[10] * view[14]);
//    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
//    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
//    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
//    glUniform3f(glGetUniformLocation(shader->ID, "cameraPos"), cam.x, cam.y, cam.z);
//
//    glBindVertexArray(VAO);
//    glActiveTexture(GL_TEXTURE0);
//    glUniform1i(glGetUniformLocation(shader->ID, "skybox"), 0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    glBindVertexArray(0);
//
//}
//
//GLuint reflectCube::loadCubemap(vector<const GLchar*> faces)
//{
//
//    GLuint textureID;
//    glGenTextures(1, &textureID);
//
//    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//    for (GLuint i = 0; i < faces.size(); i++)
//    {
//        cv::Mat img = cv::imread(faces[i], cv::IMREAD_COLOR);
//        if (img.type() == CV_8UC3)
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);
//        else if (img.type() == CV_8UC4)
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, img.cols, img.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.data);
//        img.release();
//    }
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//
//    return textureID;
//
//
