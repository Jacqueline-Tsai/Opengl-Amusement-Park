 #include <iostream>
#include <FileSystem>
#include <fstream>
#include <Fl/fl.h>
#include <windows.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glu.h>

#include "RollerCoaster.H"

RollerCoaster::RollerCoaster() {
    for (int i = 0; i < 4; i++) {
        ctrlPnts[i * 4] = glm::vec3(-1, 0, -1); ctrlPnts[i * 4 + 1] = glm::vec3(-1, 0, 1);
        ctrlPnts[i * 4 + 2] = glm::vec3(1, 0, 1); ctrlPnts[i * 4 + 3] = glm::vec3(1, 0, -1);
    }
    for (int i = 0; i < pntsNum; i++) {
        ctrlPnts[i].y = float(i) / pntsNum;
    }
    for (int i = 0; i < pntsNum; i++)rotates[i * lineSegNum] = -PI / 2.0;
    //std::ifstream fin("../src/RollerCoaster/ctrlPnt.txt");    float tmp;
    //for (int i = 0; i < pntsNum; i++) {
    //    //ctrlPnts[i] = glm::vec3();
    //    fin >> ctrlPnts[i].x >> ctrlPnts[i].y >> ctrlPnts[i].z ;
    //    rotates[i * pntsNum] = 0;
    //}
    
    //glm::mat4 M(-1.0 / 6, 0.5, -0.5, 1.0 / 6, 0.5, -1.0, 0.5, 0, -0.5, 0, 0.5, 0, 1.0 / 6, 2.0 / 3, 1.0 / 6, 0);
    glm::mat4 M(-0.8, 1.2, -1.2, 0.8, 1.6, -2.2, 1.4, -0.8, -0.8, 0, 0.8, 0, 0, 1, 0, 0);
    glm::vec3 firstPos, lastPos;
    totalLen = 0;
    for (int i = 0; i < pntsNum; i++) {
        glm::mat4 G(0);  int count = i;
        for (int j = 0; j < 4; j++) {
            G[j][0] = ctrlPnts[count].x;
            G[j][1] = ctrlPnts[count].y;
            G[j][2] = ctrlPnts[count].z;
            count = (count + 1) % pntsNum;
        }
        lastPos = G * M * glm::vec4(0, 0, 0, 1);
        if (i == 0)firstPos = lastPos;
        for (int j = 1; j <= lineSegNum; j++) {
            float t = float(j) / lineSegNum;
            int idx = i * lineSegNum + j - 1;
            rotates[idx] = rotates[i * pntsNum] * (1 - t) + rotates[(i + 1) % pntsNum * pntsNum] * t;
            glm::vec4 T(t * t * t, t * t, t, 1);
            position[idx] = G * M * T;
            xyzAxis[idx][2] = glm::normalize(position[idx] - lastPos);
            glm::mat4 rotates_matrix = glm::rotate(rotates[idx], xyzAxis[idx][2]);
            xyzAxis[idx][0] = glm::normalize(rotates_matrix * glm::vec4(glm::cross(glm::vec3(0, 1, 0), xyzAxis[idx][2]), 1));
            xyzAxis[idx][1] = glm::normalize(rotates_matrix * glm::vec4(0, 1, 0, 1));
            lineSeg[0][i * lineSegNum + j - 1].position = position[idx] + xyzAxis[idx][0] * 0.01f;  lineSeg[1][i * lineSegNum + j - 1].position = position[idx] - xyzAxis[idx][0] * 0.01f;
            totalLen += glm::length(position[idx] - lastPos);
            lengths[idx] = totalLen;
            lastPos = position[idx];
        }
    }
    //glm::vec3 xVec = glm::normalize(glm::cross(glm::vec3(0, 1, 0), firstPos - lastPos)) * 0.1f;
    //lineSeg[0][pntsNum * lineSegNum - 1].position = firstPos - xVec;  lineSeg[1][pntsNum * lineSegNum - 1].position = firstPos + xVec;
    railsNum = floor(totalLen / railInterval);
    railInterval = totalLen / railsNum;
    rails.resize(railsNum);
    for (int i = 0; i < railsNum; i++) {
        int t = std::lower_bound(lengths, lengths + infoNum, railInterval * i) - lengths;
        rails[i] = new Rail(Global::roller_coaster_model_matrix, xyzAxis[t], position[t]);
    }
    
    shader = new Shader("../src/RollerCoaster/support.vert", nullptr, nullptr, nullptr, "../src/RollerCoaster/support.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineSeg), &lineSeg[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glBindVertexArray(0);

}

void RollerCoaster::draw() {
    static clock_t time = clock();
    carPos += Global::timeInterval * speed; if (carPos > totalLen)carPos -= totalLen;

    shader->Use();
    GLfloat view[16];
    GLfloat projection[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(Global::roller_coaster_model_matrix));

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, infoNum);
    glDrawArrays(GL_LINE_STRIP, infoNum, infoNum);
    glBindVertexArray(0);

    for (int i = 0; i < railsNum; i++) {
        rails[i]->draw();
    }
    for (int i = 0; i < carsNum; i++) {
        float pos = carPos + carInterval * i;
        if (pos > totalLen)pos -= totalLen;
        int t = std::lower_bound(lengths, lengths + lineSegNum * pntsNum, pos) - lengths;
        car.draw(Global::roller_coaster_model_matrix, xyzAxis[t], position[t]);
        if (Global::camera_mode == 2 && i == 0) {
            Global::position_roller_coaster = Global::roller_coaster_model_matrix * glm::vec4((position[t] + xyzAxis[0][1] * 0.06f), 1);
            Global::lookAt_roller_coaster = xyzAxis[0][2];
            Global::up_roller_coaster = xyzAxis[0][1];
        }
        if (i == carsNum / 2) {
            speed = baseSpd * (3 - 10 * xyzAxis[t][2].y);
        }
    }
}
