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
    _model.Pre_Draw();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, _pos);
    if (_turning)
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5, 1.0, 0.0));
    if (_isLight)
        model = glm::scale(model, glm::vec3(0.5f));

    //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    //glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    glm::mat4 view = _game.GetCamera().GetViewMatrix();

    glm::mat4 projection = _game.GetCamera().GetProjMatrix();
    //projection = glm::perspective(glm::radians(cam.GetFoV()), 16.0f / 9.0f, 0.1f, 100.0f);
    //projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);
    //projection = glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f, 0.1f, 300.0f);

    glUniformMatrix4fv(glGetUniformLocation(_model.GetShaderId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(_model.GetShaderId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(_model.GetShaderId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    if (!_isLight) {
        glm::vec3 oColor(1.0f, 0.5f, 0.31f);
        glm::vec3 lColor(1.0f, 1.0f, 1.0f);
        glm::vec3 lPos = _game.GetLightList()[0]->GetPos();
        glUniform3fv(glGetUniformLocation(_model.GetShaderId(), "objectColor"), 1, &oColor[0]);
        glUniform3fv(glGetUniformLocation(_model.GetShaderId(), "lightColor"), 1, &lColor[0]);
        glUniform3fv(glGetUniformLocation(_model.GetShaderId(), "lightPos"), 1, &lPos[0]);
        glUniform3fv(glGetUniformLocation(_model.GetShaderId(), "viewPos"), 1, &_game.GetCamera().GetPos()[0]);
    }

    //std::cout << vec.x << vec.y << vec.z << std::endl;


    //std::cout << "alpha=" << alpha << std::endl;
    glUniform1f(glGetUniformLocation(_model.GetShaderId(), "mixValue"), _game.GetAlpha());

    _model.Draw();
    return 1;
}
