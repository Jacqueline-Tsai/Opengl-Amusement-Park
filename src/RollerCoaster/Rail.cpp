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

#include "Rail.H"

Rail::Rail(const glm::mat4& _model_matrix, glm::vec3 xyzAxis[3], const glm::vec3& pos) :model_matrix(_model_matrix) {
    glm::vec3 x = xyzAxis[0]*0.01f, y = xyzAxis[1] * 0.002f, z = xyzAxis[2] * 0.002f, ctr = pos;
    for (int i = 0; i < 12; i++)for (int j = 0; j < 3; j++)rail[i].vertices[j].position = ctr;
    for (float i : { -1.0f,1.0f }) {
        size_t idx1 = std::max(int(i), 0), idx2 = idx1 + 2, idx3 = idx1 + 4, idx4 = idx1 + 6, idx5 = idx1 + 8, idx6 = idx1 + 10;
        rail[idx1].vertices[0].position += x * i - y - z; rail[idx1].vertices[1].position += x * i + y - z; rail[idx1].vertices[2].position += x * i - y + z;
        rail[idx2].vertices[0].position += x * i + y + z; rail[idx2].vertices[1].position += x * i + y - z; rail[idx2].vertices[2].position += x * i - y + z;

        rail[idx3].vertices[0].position += z * i - y - x; rail[idx3].vertices[1].position += z * i + y - x; rail[idx3].vertices[2].position += z * i - y + x;
        rail[idx4].vertices[0].position += z * i + y + x; rail[idx4].vertices[1].position += z * i + y - x; rail[idx4].vertices[2].position += z * i - y + x;

        rail[idx5].vertices[0].position += i * y - x - z; rail[idx5].vertices[1].position += i * y + x - z; rail[idx5].vertices[2].position += i * y - x + z;
        rail[idx6].vertices[0].position += i * y + x + z; rail[idx6].vertices[1].position += i * y + x - z; rail[idx6].vertices[2].position += i * y - x + z;
    }
    for (int i = 0; i < 12; i++) {
        glm::vec3 normal = glm::cross(rail[i].vertices[0].position - rail[i].vertices[1].position, rail[i].vertices[0].position - rail[i].vertices[2].position);
        for (int j = 0; j < 3; j++)rail[i].vertices[j].normal = normal;
    }

    shader = new Shader("../src/RollerCoaster/Rail.vert", nullptr, nullptr, nullptr, "../src/RollerCoaster/Rail.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rail), &rail, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glBindVertexArray(0);
}

void Rail::draw() {
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
}
