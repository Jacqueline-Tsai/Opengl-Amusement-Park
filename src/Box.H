#ifndef BOX_H
#define BOX_H

#include <vector>
#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"
#include "RenderUtilities/Mesh.h"

class Box {
public:
    Box();
    void init();
    void draw();

    bool loaded = false;
    Shader* shader = nullptr;
    TextureCubeMap* texture = nullptr;
    std::vector<const GLchar*> fileNames;
    GLuint VAO, VBO, EBO;
    GLfloat vertices[108] = {
        -50.0f,  50.0f, -50.0f,
        -50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,

        -50.0f, -50.0f,  50.0f,
        -50.0f, -50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f,  50.0f,
        -50.0f, -50.0f,  50.0f,

         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,

        -50.0f, -50.0f,  50.0f,
        -50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f, -50.0f,  50.0f,
        -50.0f, -50.0f,  50.0f,


        /*-50.0f,  50.0f, -50.0f,
         50.0f,  50.0f, -50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
        -50.0f,  50.0f,  50.0f,
        -50.0f,  50.0f, -50.0f,*/

        -50.0f, -50.0f, -50.0f,
        -50.0f, -50.0f,  50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
        -50.0f, -50.0f,  50.0f,
         50.0f, -50.0f,  50.0f
    };
};

#endif