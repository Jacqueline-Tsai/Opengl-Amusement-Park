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
#include <cstdlib>
#include "Terrain.H"

Terrain::Terrain() {
    for (int i = 0; i < size; i++)vertices[i] = 0;
    terrain = { terrain0 ,terrain1 ,terrain2 ,terrain3 ,terrain4 };

    shader = new Shader("../src/shaders/terrain.vert", nullptr, nullptr, nullptr, "../src/shaders/terrain.frag");
    texture = new Texture2D("../Images/terrain/concrete.jpg");
    genHeight();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (Global::terrain_lv == 0)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain0), &terrain0, GL_STATIC_DRAW);
    if (Global::terrain_lv == 1)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain1), &terrain1, GL_STATIC_DRAW);
    if (Global::terrain_lv == 2)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain2), &terrain2, GL_STATIC_DRAW);
    if (Global::terrain_lv == 3)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain3), &terrain3, GL_STATIC_DRAW);
    if (Global::terrain_lv == 4)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain4), &terrain4, GL_STATIC_DRAW);
    if (Global::terrain_lv == 5)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain5), &terrain5, GL_STATIC_DRAW);
    if (Global::terrain_lv == 6)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain6), &terrain6, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glBindVertexArray(0);
}
float Terrain::genRand(int lv) {
    float offset;
    while (1) {
        offset = float(rand() % 2000 - 1000) * pow(Global::d_offset, lv) * 0.001 * Global::offset_max;
        if (offset != 0)break;;
    }
    return offset;
}
void Terrain::genHeight() {

    float h[4][4] = { {1,1,1,1},{1,0,0,1},{1,0,0,1},{1,1,1,1} };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int idx = 2 * (i * 3 + j);
            if (i == j) {
                terrain0[idx].vertices[0].position = glm::vec3(i, h[i][j], j);
                terrain0[idx].vertices[1].position = glm::vec3(i + 1, h[i + 1][j], j);
                terrain0[idx].vertices[2].position = glm::vec3(i + 1, h[i + 1][j + 1], j + 1);
                terrain0[idx + 1].vertices[0].position = glm::vec3(i, h[i][j], j);
                terrain0[idx + 1].vertices[1].position = glm::vec3(i, h[i][j + 1], j + 1);
                terrain0[idx + 1].vertices[2].position = glm::vec3(i + 1, h[i + 1][j + 1], j + 1);
            } else {
                terrain0[idx].vertices[0].position = glm::vec3(i, h[i][j], j);
                terrain0[idx].vertices[1].position = glm::vec3(i + 1, h[i + 1][j], j);
                terrain0[idx].vertices[2].position = glm::vec3(i, h[i][j + 1], j + 1);
                terrain0[idx + 1].vertices[0].position = glm::vec3(i + 1, h[i + 1][j], j);
                terrain0[idx + 1].vertices[1].position = glm::vec3(i, h[i][j + 1], j + 1);
                terrain0[idx + 1].vertices[2].position = glm::vec3(i + 1, h[i + 1][j + 1], j + 1);
            }
        }
    }
    std::unordered_map<float, std::unordered_map<float, float>> log;
    for (int i = 0; i < maxLv; i++) {
        Triangle* bef, * aft;
        if (i == 0) { bef = terrain0; aft = terrain1; }
        else if (i == 1) {  bef = terrain1; aft = terrain2; }
        else if (i == 2) {  bef = terrain2; aft = terrain3; }
        else if (i == 3) {  bef = terrain3; aft = terrain4;  }
        else if (i == 4) {  bef = terrain4; aft = terrain5; }
        else if (i == 5) {  bef = terrain5; aft = terrain6; }
        for (int j = 0; j < 18 * pow(4, i); j++) {//j th triangle
            glm::vec3 befPos[3] = { bef[j].vertices[0].position ,bef[j].vertices[1].position ,bef[j].vertices[2].position };
            glm::vec3 midPos[3] = { glm::vec3((befPos[0].x + befPos[1].x) / 2,(befPos[0].y + befPos[1].y) / 2,(befPos[0].z + befPos[1].z) / 2),
                glm::vec3((befPos[0].x + befPos[2].x) / 2,(befPos[0].y + befPos[2].y) / 2,(befPos[0].z + befPos[2].z) / 2),
                glm::vec3((befPos[2].x + befPos[1].x) / 2,(befPos[2].y + befPos[1].y) / 2,(befPos[2].z + befPos[1].z) / 2) };
            for (int k = 0; k < 3; k++) {
                midPos[k].y = log.find(midPos[k].x) != log.end() && log[midPos[k].x].find(midPos[k].z) != log[midPos[k].x].end() ? log[midPos[k].x][midPos[k].z] : midPos[k].y + genRand(i);
                log[midPos[k].x][midPos[k].z] = midPos[k].y;
            }
            aft[j * 4].vertices[0].position = befPos[0];    aft[j * 4].vertices[1].position = midPos[0];    aft[j * 4].vertices[2].position = midPos[1];
            aft[j * 4 + 1].vertices[0].position = befPos[1];    aft[j * 4 + 1].vertices[1].position = midPos[0];    aft[j * 4 + 1].vertices[2].position = midPos[2];
            aft[j * 4 + 2].vertices[0].position = befPos[2];    aft[j * 4 + 2].vertices[1].position = midPos[1];    aft[j * 4 + 2].vertices[2].position = midPos[2];
            aft[j * 4 + 3].vertices[0].position = midPos[0];    aft[j * 4 + 3].vertices[1].position = midPos[1];    aft[j * 4 + 3].vertices[2].position = midPos[2];
        }
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 3; j++) {
            glm::vec3 pos = terrain6[i].vertices[j].position;
            Global::terrain_height[int(pos.x * 64)][int(pos.z * 64)] = pos.y;
        }
    }
    
}

void Terrain::draw() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (Global::terrain_lv == 0)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain0), &terrain0, GL_STATIC_DRAW);
    if (Global::terrain_lv == 1)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain1), &terrain1, GL_STATIC_DRAW);
    if (Global::terrain_lv == 2)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain2), &terrain2, GL_STATIC_DRAW);
    if (Global::terrain_lv == 3)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain3), &terrain3, GL_STATIC_DRAW);
    if (Global::terrain_lv == 4)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain4), &terrain4, GL_STATIC_DRAW);
    if (Global::terrain_lv == 5)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain5), &terrain5, GL_STATIC_DRAW);
    if (Global::terrain_lv == 6)    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain6), &terrain6, GL_STATIC_DRAW);

    shader->Use();
    GLfloat view[16];
    GLfloat projection[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(Global::terrain_model_matrix));
    glUniform1i(glGetUniformLocation(shader->ID, "texture"), 0);

    glBindVertexArray(VAO);
    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, int(54 * pow(4, Global::terrain_lv)));
    glBindVertexArray(0);

}
