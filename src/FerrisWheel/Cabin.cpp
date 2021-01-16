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

#include "Cabin.H"

Cabin::Cabin(){
    vector<int> pnt = { -3,-1 };
    for (int i = -1; i < 2; i += 2) {
        size_t idx1 = std::max(i, 0), idx2 = idx1 + 2, idx3 = idx1 + 4, idx4 = idx1 + 6, idx5 = idx1 + 8, idx6 = idx1 + 10;
        box[idx1].vertices[0].position = glm::vec3(i, -1, -1); box[idx1].vertices[1].position = glm::vec3(i, -3, -1); box[idx1].vertices[2].position = glm::vec3(i, -1, 1);
        box[idx2].vertices[0].position = glm::vec3(i, -3, 1); box[idx2].vertices[1].position = glm::vec3(i, -3, -1); box[idx2].vertices[2].position = glm::vec3(i, -1, 1);

        box[idx3].vertices[0].position = glm::vec3(-1, -1, i); box[idx3].vertices[1].position = glm::vec3(-1, -3, i); box[idx3].vertices[2].position = glm::vec3(1, -1, i);
        box[idx4].vertices[0].position = glm::vec3(1, -3, i); box[idx4].vertices[1].position = glm::vec3(-1, -3, i); box[idx4].vertices[2].position = glm::vec3(1, -1, i);

        box[idx5].vertices[0].position = glm::vec3(-1, i - 2, -1); box[idx5].vertices[1].position = glm::vec3(1, i - 2, -1); box[idx5].vertices[2].position = glm::vec3(-1, i - 2, 1);
        box[idx6].vertices[0].position = glm::vec3(1, i - 2, 1); box[idx6].vertices[1].position = glm::vec3(1, i - 2, -1); box[idx6].vertices[2].position = glm::vec3(-1, i - 2, 1);
    }
    for (int i = 0; i < 12; i++) {
        glm::vec3 normal = glm::cross(box[i].vertices[0].position - box[i].vertices[1].position, box[i].vertices[0].position - box[i].vertices[2].position);
        for (int j = 0; j < 3; j++)box[i].vertices[j].normal = normal;
    }
    
    shader = new Shader("../src/FerrisWheel/Cabin.vert", nullptr, nullptr, nullptr, "../src/FerrisWheel/Cabin.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box), &box, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glBindVertexArray(0);
}


void Cabin::draw(glm::mat4& model_matrix, bool camera) {
    model_matrix = model_matrix * glm::mat4(0.3, 0, 0, 0, 0, 0.3, 0, 0, 0, 0, 0.3, 0, 0, 0, 0, 1);
    shader->Use();
    GLfloat view[16];
    GLfloat projection[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    if (Global::camera_mode == 1 && camera) {
        Global::position_ferris_wheel = model_matrix * glm::vec4(0, -0.5, 0, 1);
    }
}
