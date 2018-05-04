#include "AModel.h"

#include <iostream>

unsigned int AModel::makeShader(GLenum type, const char **source) {
    unsigned int shader;
    shader = glCreateShader(type);

    glShaderSource(shader, 1, source, NULL);
    glCompileShader(shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        else if (type == GL_FRAGMENT_SHADER)
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

    }
    return shader;
}

unsigned int AModel::linkShaders(unsigned int *shadProgs, unsigned int n) {
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    for (unsigned int i = 0; i<n; i++)
        glAttachShader(shaderProgram, shadProgs[i]);

    glLinkProgram(shaderProgram);

    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    for (unsigned int i = 0; i<n; i++)
        glDeleteShader(shadProgs[i]);

    std::cout << "shaderProgram=" << shaderProgram << std::endl;

    return shaderProgram;
}
