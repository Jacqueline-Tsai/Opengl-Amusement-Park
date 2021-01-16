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

#include "Car.H"

Car::Car() {
    texture = new Texture2D("../Images/wood.jpg");
    shader = new Shader("../src/RollerCoaster/Car.vert", nullptr, nullptr, nullptr, "../src/RollerCoaster/Car.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(car), &car, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
}


void Car::draw(glm::mat4& model_matrix, glm::vec3 xyzAxis[3], const glm::vec3& pos) {
    glm::vec3 x = xyzAxis[0] * 0.01f, y = xyzAxis[1] * 0.02f, z = xyzAxis[2] * 0.02f, ctr = pos + y * 1.5f;
    for (int i = 0; i < 12; i++)for (int j = 0; j < 3; j++)car[i].vertices[j].position = ctr;
    for (float i : { -1.0f, 1.0f }) {
        size_t idx1 = std::max(int(i), 0), idx2 = idx1 + 2, idx3 = idx1 + 4, idx4 = idx1 + 6, idx5 = idx1 + 8, idx6 = idx1 + 10;
        /*car[idx1].vertices[0].position += x * i - y - z; car[idx1].vertices[1].position += x * i + y - z; car[idx1].vertices[2].position += x * i - y + z;
        car[idx2].vertices[0].position += x * i + y + z; car[idx2].vertices[1].position += x * i + y - z; car[idx2].vertices[2].position += x * i - y + z;

        car[idx3].vertices[0].position += z * i - y - x; car[idx3].vertices[1].position += z * i + y - x; car[idx3].vertices[2].position += z * i - y + x;
        car[idx4].vertices[0].position += z * i + y + x; car[idx4].vertices[1].position += z * i + y - x; car[idx4].vertices[2].position += z * i - y + x;

        car[idx5].vertices[0].position += i * y - x - z; car[idx5].vertices[1].position += i * y + x - z; car[idx5].vertices[2].position += i * y - x + z;
        car[idx6].vertices[0].position += i * y + x + z; car[idx6].vertices[1].position += i * y + x - z; car[idx6].vertices[2].position += i * y - x + z;*/
        car[idx1].vertices[0].position = glm::vec3(i, -1, -1); car[idx1].vertices[1].position = glm::vec3(i, 1, -1); car[idx1].vertices[2].position = glm::vec3(i, -1, 1);
        car[idx2].vertices[0].position = glm::vec3(i, 1, 1); car[idx2].vertices[1].position = glm::vec3(i, 1, -1); car[idx2].vertices[2].position = glm::vec3(i, -1, 1);

        car[idx3].vertices[0].position = glm::vec3(-1, -1, i);  car[idx3].vertices[1].position = glm::vec3(-1, 1, i); car[idx3].vertices[2].position = glm::vec3(1, -1, i);;
        car[idx4].vertices[0].position = glm::vec3(1, 1, i); car[idx4].vertices[1].position = glm::vec3(-1, 1, i); car[idx4].vertices[2].position = glm::vec3(1, -1, i);

        car[idx5].vertices[0].position = glm::vec3(-1, i, -1); car[idx5].vertices[1].position = glm::vec3(1, i, -1); car[idx5].vertices[2].position = glm::vec3(-1, i, 1);
        car[idx6].vertices[0].position = glm::vec3(1, i, 1); car[idx6].vertices[1].position = glm::vec3(1, i, -1); car[idx6].vertices[2].position = glm::vec3(-1, i, 1);
    }
    for (int i = 0; i < 12; i++) {
        glm::vec3 normal = glm::cross(car[i].vertices[0].position - car[i].vertices[1].position, car[i].vertices[0].position - car[i].vertices[2].position);
        for (int j = 0; j < 3; j++)car[i].vertices[j].normal = normal;
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(car), &car, GL_STATIC_DRAW);

    shader->Use();
    GLfloat view[16];
    GLfloat projection[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
    glUniform3fv(glGetUniformLocation(shader->ID, "x"), 1, &x[0]);
    glUniform3fv(glGetUniformLocation(shader->ID, "y"), 1, &y[0]);
    glUniform3fv(glGetUniformLocation(shader->ID, "z"), 1, &z[0]);
    glUniform3fv(glGetUniformLocation(shader->ID, "center"), 1, &ctr[0]); 
    glUniform1i(glGetUniformLocation(shader->ID, "Texture"), 0);
    texture->bind(0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

