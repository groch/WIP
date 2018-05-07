#include "CGame.h"

#include "shader.hpp"

int main() {
    CGame game;

    std::cout << "Game built" << std::endl;

    Shader shader("G:/code/bidouille/WIP/shaders/vertexShader1.shader", "G:/code/bidouille/WIP/shaders/fragmentShader1.shader");

    std::cout << "Shader built" << std::endl;

    game.loop();
    return 0;
}
