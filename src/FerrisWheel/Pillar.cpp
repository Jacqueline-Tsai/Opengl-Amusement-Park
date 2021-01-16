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

#include "Pillar.H"

Pillar::Pillar(int _lv,bool _camera) :lv(_lv), camera(_camera){
    time = clock();
    speed = Global::ferris_wheel_speed * pow(-2, lv);
    if (lv == Global::ferris_wheel_max_lv) {
        cabin = new Cabin();
    }
    else {
        pillar.resize(Global::ferris_wheel_cars_number);
        pillar[0] = new Pillar(lv + 1, true);
        for (int i = 1; i < Global::ferris_wheel_cars_number; i++) {
            pillar[i] = new Pillar(lv + 1);
        }
    }
    support[0].position = glm::vec3(0, 0, 0.09);    support[1].position = glm::vec3(0.5, 0, 0.03);
    support[2].position = glm::vec3(0, 0, -0.09);    support[3].position = glm::vec3(0.5, 0, -0.03);
    support[4].position = glm::vec3(0.5, 0, 0.03);   support[5].position = glm::vec3(0.5, 0, -0.03);

    shader = new Shader("../src/FerrisWheel/pillar.vert", nullptr, nullptr, nullptr, "../src/FerrisWheel/pillar.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(support), &support[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glBindVertexArray(0);
}

void Pillar::draw(const glm::mat4& model_matrix) {
    t += Global::timeInterval * speed;

    shader->Use();
    GLfloat view[16];
    GLfloat projection[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);

    if (lv == Global::ferris_wheel_max_lv) {
        glm::vec4 pnt = model_matrix * glm::vec4(0.5, 0, 0, 1);
        cabin->draw(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, pnt.x, pnt.y, pnt.z, 1), camera);
    }
    else {
        for (int i = 0; i < Global::ferris_wheel_cars_number; i++) {
            float theta = float(i) / Global::ferris_wheel_cars_number * 2 * PI + t, ratio = 1.0 / 3;
            pillar[i]->draw(model_matrix *
                glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0.5, 0, 0, 1) *
                glm::mat4(cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) *
                glm::mat4(ratio, 0, 0, 0, 0, ratio, 0, 0, 0, 0, ratio, 0, 0, 0, 0, 1));
        }
    }
}
