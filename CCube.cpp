#include "CCube.h"
#include "render.h"

#include <iostream>
#include <fstream>
#include <sstream>

CCube::CCube()
{
    //ctor
        //ctor
    float vertices[] = {
    // positions                // normal           // colors         // texture coord
    //face
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left
    // gauche
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left
    //droite
         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left
    //bas
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left
    //haut
         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left
    //derriere
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left
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

    std::ifstream vShader;
    std::ifstream fShader;

    std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
    vShader.open("G:/code/bidouille/WIP/shaders/vertexShader1.shader");
    fShader.open("G:/code/bidouille/WIP/shaders/fragmentShader1.shader");

    vShaderStream << vShader.rdbuf();
    fShaderStream << fShader.rdbuf();

    vShader.close();
    fShader.close();

    unsigned int myShader[2];

    std::string vertexShaderSource = vShaderStream.str();
    const char* vertexShaderSource_cs = vertexShaderSource.c_str();
    myShader[0] = AModel::makeShader(GL_VERTEX_SHADER, &vertexShaderSource_cs);

    std::string fragmentShaderSource = fShaderStream.str();
    const char* fragmentShaderSource_cs = fragmentShaderSource.c_str();
    myShader[1] = makeShader(GL_FRAGMENT_SHADER, &fragmentShaderSource_cs);

    _shaderProgram = linkShaders(myShader, 2);

    glUseProgram(_shaderProgram);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    _texId[0] = loadImage("ressources\\textures\\container2.png");
    glUniform1i(glGetUniformLocation(_shaderProgram, "material.texture_diffuse[0]"), 0);
    _texId[1] = loadImage("ressources\\textures\\matrix.jpg");
    glUniform1i(glGetUniformLocation(_shaderProgram, "material.texture_emission[0]"), 1);
    _texId[2] = loadImage("ressources\\textures\\container2_specular.png");
    glUniform1i(glGetUniformLocation(_shaderProgram, "material.texture_specular[0]"), 2);

//    std::cout << "texId[0]=" << _texId[0] << std::endl;
//    std::cout << "texId[1]=" << _texId[1] << std::endl;
}

CCube::~CCube()
{
    //dtor
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

void    CCube::Draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texId[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texId[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _texId[2]);

    glBindVertexArray(_vao);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
