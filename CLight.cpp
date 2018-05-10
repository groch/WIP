#include "CLight.h"
#include "render.h"

#include <iostream>
#include <fstream>
#include <sstream>

CLight::CLight() : _vao(), _vbo(), _ebo()
{
    //ctor
        //ctor
    float vertices[] = {
    // positions         // colors         // texture coord
    //face
         0.5f,  0.5f,  0.5f, // top right
         0.5f, -0.5f,  0.5f, // bottom right
        -0.5f, -0.5f,  0.5f, // bottom left
        -0.5f,  0.5f,  0.5f, // top left
    // gauche
        -0.5f,  0.5f,  0.5f, // top right
        -0.5f, -0.5f,  0.5f, // bottom right
        -0.5f, -0.5f, -0.5f, // bottom left
        -0.5f,  0.5f, -0.5f, // top left
    //droite
         0.5f,  0.5f, -0.5f, // top right
         0.5f, -0.5f, -0.5f, // bottom right
         0.5f, -0.5f,  0.5f, // bottom left
         0.5f,  0.5f,  0.5f, // top left
    //bas
         0.5f, -0.5f,  0.5f, // top right
         0.5f, -0.5f, -0.5f, // bottom right
        -0.5f, -0.5f, -0.5f, // bottom left
        -0.5f, -0.5f,  0.5f, // top left
    //haut
         0.5f,  0.5f, -0.5f, // top right
         0.5f,  0.5f,  0.5f, // bottom right
        -0.5f,  0.5f,  0.5f, // bottom left
        -0.5f,  0.5f, -0.5f, // top left
    //derriere
         0.5f,  0.5f, -0.5f, // top right
         0.5f, -0.5f, -0.5f, // bottom right
        -0.5f, -0.5f, -0.5f, // bottom left
        -0.5f,  0.5f, -0.5f, // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,

        4, 5, 7,
        5, 6, 7,

        8, 9, 11,
        9, 10, 11,

        12, 13, 15,
        13, 14, 15,

        16, 17, 19,
        17, 18, 19,

        20, 21, 23,
        21, 22, 23,

        24, 25, 27,
        25, 26, 27
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

//    _texId[0] = loadImage("ressources\\textures\\wall.jpg");
//    glUniform1i(glGetUniformLocation(_shaderProgram, "texture1"), 0);
//    _texId[1] = loadImage("ressources\\textures\\awesomeface.png");
//    glUniform1i(glGetUniformLocation(_shaderProgram, "texture2"), 1);

//    std::cout << "texId[0]=" << _texId[0] << std::endl;
//    std::cout << "texId[1]=" << _texId[1] << std::endl;
}

CLight::~CLight() throw ()
{
    //dtor
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

void    CLight::Draw(Shader& shader) {
    shader.use();
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, _texId[0]);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, _texId[1]);

    glBindVertexArray(_vao);

    //glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
