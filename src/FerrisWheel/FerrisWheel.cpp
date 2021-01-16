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

#include "FerrisWheel.H"

FerrisWheel::FerrisWheel() {
    pillar.resize(Global::ferris_wheel_cars_number);
    for (int i = 0; i < Global::ferris_wheel_cars_number; i++) {
        pillar[i] = new Pillar();
    }
    float minHeight = 1000;
    for (int j = 0; j < 4; j++) {
        float x = j % 2 ? 0.3 : -0.3, z = j / 2 ? 0.09 : -0.09;
        support[j * 2].position = glm::vec3(x, -0.3, z);
        support[j * 2 + 1].position = glm::vec3(0, 0.7, z);
        glm::vec4 globalPos = Global::ferris_wheel_model_matrix * glm::vec4(support[j * 2].position, 1);
        minHeight = fmin(minHeight, Global::get_terrain_height(globalPos.x, globalPos.z));
    }
    support[8].position = glm::vec3(0, 0.7, -0.1);   support[9].position = glm::vec3(0, 0.7, 0.1);
    Global::ferris_wheel_model_matrix[3][1] = minHeight - (Global::ferris_wheel_model_matrix * glm::vec4(0, -0.3, 0, 1)).y;

    //float ratio = 0.05, last_x = sin(PI * 19 / 10) * ratio, last_z = cos(PI * 19 / 10) * ratio;
    //for (int i = 0; i < 20; i++) {
    //    float off_x = sin(PI * i / 10) * ratio, off_z = cos(PI * i / 10) * ratio;
    //    for (int j = 0; j < 4; j++) {
    //        float x = j % 2 ? 1 : -1, z = j / 2 ? 1 : -1;
    //        int idx = i * 2 + j * 40;
    //        support[idx].vertices[0].position = glm::vec3(x + off_x, 0, z + off_z);
    //        support[idx].vertices[1].position = glm::vec3(last_x, 0.55, z + last_z);
    //        support[idx].vertices[2].position = glm::vec3(x + last_x, 0, z + last_z);
    //        support[idx + 1].vertices[0].position = glm::vec3(x + off_x, 0, z + off_z);
    //        support[idx + 1].vertices[1].position = glm::vec3(last_x, 0.55, z + last_z);
    //        support[idx + 1].vertices[2].position = glm::vec3(off_x, 0.55, z + off_z);
    //    }
    //    /*support[i * 10 + 8].vertices[0].position = glm::vec3(0, 0, 0);
    //    support[i * 10 + 8].vertices[1].position = glm::vec3(0, 0, 0);
    //    support[i * 10 + 8].vertices[2].position = glm::vec3(0, 0, 0);
    //    support[i * 10 + 9].vertices[0].position = glm::vec3(0, 0, 0);
    //    support[i * 10 + 9].vertices[1].position = glm::vec3(0, 0, 0);
    //    support[i * 10 + 9].vertices[2].position = glm::vec3(0, 0, 0);*/
    //    last_x = off_x; last_z = off_z;
    //}
    //for (int i = 0; i < 200; i++) {
    //    glm::vec3 normal = glm::cross(support[i].vertices[0].position - support[i].vertices[1].position, support[i].vertices[0].position - support[i].vertices[2].position); 
    //    for (int j = 0; j < 3; j++)support[i].vertices[j].normal = normal;
    //}

    shader = new Shader("../src/FerrisWheel/support.vert", nullptr, nullptr, nullptr, "../src/FerrisWheel/support.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(support), &support[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    /*glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));*/
    glBindVertexArray(0);
}

void FerrisWheel::draw() {
    t += Global::timeInterval * Global::ferris_wheel_speed;

    shader->Use();
    GLfloat view[16];
    GLfloat projection[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(Global::ferris_wheel_model_matrix));
    glUniform1i(glGetUniformLocation(shader->ID, "texture"), 0);
    glUniform1i(glGetUniformLocation(shader->ID, "heightmap"), 1);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 10);
    glBindVertexArray(0);

    for (int i = 0; i < Global::ferris_wheel_cars_number; i++) {
        float theta = float(i) / Global::ferris_wheel_cars_number * 2 * PI + t;
        float debug = cos(theta);
        pillar[i]->draw(Global::ferris_wheel_model_matrix *
            glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0.7, 0, 1) *
            glm::mat4(cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) );
    }
}
