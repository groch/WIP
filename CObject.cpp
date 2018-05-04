#include "CObject.h"

#include <iostream>
#include <fstream>
#include <math.h>

#include "IL\ilut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render.h"

#include <GLFW\glfw3.h>

#include "CGame.h"

CObject::CObject(CGame& game, AModel& model, glm::vec3 pos, bool turning, bool isLight) : _model(model), _turning(turning), _pos(pos), _isLight(isLight), _game(game)
{
    //ctor
}

CObject::~CObject()
{
    //dtor
}

int CObject::Draw() {
    _model.UseShaderProgram();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, _pos);
    if (_turning)
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5, 1.0, 0.0));
    if (_isLight)
        model = glm::scale(model, glm::vec3(0.5f));

    glm::mat4 view = _game.GetCamera().GetViewMatrix();
    glm::mat4 projection = _game.GetCamera().GetProjMatrix();

    _model.SetMat4("model", model);
    _model.SetMat4("view", view);
    _model.SetMat4("projection", projection);

    if (!_isLight) {
        // random light to test
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

//        _model.SetVec3("material.ambient",  1.0f, 0.5f, 0.31f);
//        _model.SetVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
//        _model.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
//        _model.SetFloat("material.shininess", 32.0f);

        _model.SetVec3("material.ambient",  0.0215f, 0.1745f, 0.0215f);
        _model.SetVec3("material.diffuse",  0.07568f, 0.61424f, 0.07568f);
        _model.SetVec3("material.specular", 0.633f, 0.727811f, 0.633f);
        _model.SetFloat("material.shininess", 0.6f * 128.0f);

        _model.SetVec3("light.ambient",  0.01f, 0.01f, 0.01f);
        //_model.SetVec3("light.ambient",  1.0f, 1.0f, 1.0f);
        //_model.SetVec3("light.ambient",  ambientColor);

        _model.SetVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
        //_model.SetVec3("light.diffuse",  1.0f, 1.0f, 1.0f);
        //_model.SetVec3("light.diffuse",  diffuseColor);

        _model.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
        _model.SetVec3("light.position", _game.GetLightList()[0]->GetPos());
        _model.SetVec3("viewPos", _game.GetCamera().GetPos());
        _model.SetFloat("time", glfwGetTime());
    }

    //std::cout << vec.x << vec.y << vec.z << std::endl;


    //std::cout << "alpha=" << alpha << std::endl;
    glUniform1f(glGetUniformLocation(_model.GetShaderId(), "mixValue"), _game.GetAlpha());

    _model.Draw();
    return 1;
}
