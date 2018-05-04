#include "AModel.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

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

void AModel::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(_shaderProgram, name.c_str()), (int)value);
}

void AModel::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(_shaderProgram, name.c_str()), value);
}

void AModel::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(_shaderProgram, name.c_str()), value);
}

void AModel::SetVec2(const std::string &name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, &value[0]);
}

void AModel::SetVec3(const std::string &name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, &value[0]);
}

void AModel::SetVec4(const std::string &name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, &value[0]);
}

void AModel::SetMat2(const std::string &name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void AModel::SetMat3(const std::string &name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void AModel::SetMat4(const std::string &name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
