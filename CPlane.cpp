#include "CPlane.h"
#include "render.h"

#include <iostream>
#include <fstream>
#include <sstream>

CPlane::CPlane(unsigned int planeId) : _vao(), _vbo(), _ebo()
{
    //ctor
        //ctor
    float vertices[] = {
    // positions                // normal         // texture coord
    //fa
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,   0.0f,  1.0f,  0.0f, // top right
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,   0.0f,  1.0f,  0.0f, // bottom right
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,   0.0f,  1.0f,  0.0f, // bottom left
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,   0.0f,  1.0f,  0.0f  // top left
    };

    unsigned int indices[] = {
        0, 3, 2,
        2, 1, 0
    };

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(_vao);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8* sizeof(float)));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11* sizeof(float)));
    glEnableVertexAttribArray(4);

        // vertex tangent
//        glEnableVertexAttribArray(3);
//        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
//        // vertex bitangent
//        glEnableVertexAttribArray(4);
//        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    if (planeId == 0) {
        _texId[0] = loadImage("ressources/textures/196.jpg");
        _texId[1] = loadImage("ressources/textures/196_norm.jpg");
        _texId[2] = loadImage("ressources/textures/196_disp.jpg");
        //_texId[2] = 0;
    } else if (planeId == 1) {
        _texId[0] = loadImage("ressources/textures/bricks2.jpg");
        _texId[1] = loadImage("ressources/textures/bricks2_normal.jpg");
        _texId[2] = loadImage("ressources/textures/bricks2_disp.jpg");
    } else if (planeId == 2) {
        _texId[0] = loadImage("ressources/textures/awesomeface.png");
        _texId[1] = loadImage("ressources/textures/awesomeface_NRM.png");
        _texId[2] = loadImage("ressources/textures/awesomeface_DISP.png");
    }
}

CPlane::~CPlane()
{
    //dtor
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

void    CPlane::Draw(Shader& shader) {
    shader.use();
    shader.setInt("material.texture_emission_count", 0);
    shader.setInt("material.texture_specular_count", 0);
    glActiveTexture(GL_TEXTURE0);
    shader.setInt("material.texture_diffuse_count", 1);
    shader.setInt("material.texture_diffuse[0]", 0);
    glBindTexture(GL_TEXTURE_2D, _texId[0]);
    glActiveTexture(GL_TEXTURE1);
    shader.setInt("material.texture_normal_count", 1);
    shader.setInt("material.texture_normal[0]", 1);
    glBindTexture(GL_TEXTURE_2D, _texId[1]);
    if (_texId[2]) {
        glActiveTexture(GL_TEXTURE2);
        shader.setInt("material.texture_height_count", 1);
        shader.setInt("material.texture_height[0]", 2);
        glBindTexture(GL_TEXTURE_2D, _texId[2]);
    }

    shader.setFloat("material.shininess", 0.6f * 32.0f);

    glBindVertexArray(_vao);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
