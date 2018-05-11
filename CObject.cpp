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

CObject::CObject(CGame& game, IModel& model, glm::vec3 pos, glm::vec3 scale, bool turning, bool isLight) : _model(model), _turning(turning), _pos(pos), _scale(scale), _isLight(isLight), _game(game)
{
    //ctor
}

CObject::~CObject()
{
    //dtor
}

int CObject::Draw(Shader& shader, glm::vec3 scale) {
    //_model.UseShaderProgram();
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, _pos);
    if (_turning)
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, _scale);
    model = glm::scale(model, scale);

    glm::mat4 view = _game.GetCamera().GetViewMatrix();
    glm::mat4 projection = _game.GetCamera().GetProjMatrix();

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    if (!_isLight) {
        // random light to test
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.3f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.33333f); // low influence

        //color
//        _model.SetVec3("material.ambient",  1.0f, 0.5f, 0.31f);
//        _model.SetVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
//        _model.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
//        _model.SetFloat("material.shininess", 32.0f);

//        _model.SetVec3("material.ambient",  0.0215f, 0.1745f, 0.0215f);
//        _model.SetVec3("material.diffuse",  0.07568f, 0.61424f, 0.07568f);
//        _model.SetVec3("material.specular", 0.633f, 0.727811f, 0.633f);
//        shader.setFloat("material.shininess", 0.6f * 128.0f);

        //_model.SetVec3("dirLight.ambient",  ambientColor);
        //_model.SetVec3("dirLight.diffuse",  diffuseColor);
        shader.setVec3("dirLight.ambient", 0.01f, 0.01f, 0.01f);
        shader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
        shader.setVec3("dirLight.direction", (float)sin(glfwGetTime()*0.5), 0.0f, -1.0f);
        //shader.setVec3("dirLight.direction", 0.0f, 0.0f, 1.0f);


        shader.setInt("nbPointLight", 1);

        shader.setVec3("pointLights[0].ambient",  0.01f, 0.01f, 0.01f);
        shader.setVec3("pointLights[0].diffuse",  0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
        shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        shader.setVec3("pointLights[0].position", _game.GetLightList()[0]->GetPos());

        //distance
        shader.setFloat("pointLights[0].constant",  1.0f);
        shader.setFloat("pointLights[0].linear",    0.09f);
        shader.setFloat("pointLights[0].quadratic", 0.032f);
        //_model.SetVec3("light.ambient",  1.0f, 1.0f, 1.0f);
        //_model.SetVec3("light.ambient",  ambientColor);

        //_model.SetVec3("light.diffuse",  1.0f, 1.0f, 1.0f);
        //_model.SetVec3("light.diffuse",  diffuseColor);

        //light type
        shader.setVec3("spotLight.ambient",  0.01f, 0.01f, 0.01f);
        shader.setVec3("spotLight.diffuse",  1.0f, 1.0f, 1.0f); // darken the light a bit to fit the scene
        shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

        shader.setVec3("spotLight.position", _game.GetCamera().GetPos());
        shader.setVec3("spotLight.direction", _game.GetCamera().GetFront());
        shader.setFloat("spotLight.cutOff",   glm::cos(glm::radians(7.5f)));
        shader.setFloat("spotLight.outerCutOff",   glm::cos(glm::radians(17.5f)));

        //distance
        shader.setFloat("spotLight.constant",  1.0f);
        shader.setFloat("spotLight.linear",    0.09f);
        shader.setFloat("spotLight.quadratic", 0.032f);

        shader.setVec3("viewPos", _game.GetCamera().GetPos());
        shader.setFloat("time", glfwGetTime());
    }

    //std::cout << vec.x << vec.y << vec.z << std::endl;


    //std::cout << "alpha=" << alpha << std::endl;
    shader.setFloat("mixValue", _game.GetAlpha());

    _model.Draw(shader);
    return 1;
}
