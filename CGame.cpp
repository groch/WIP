#include "CGame.h"
#include "CPerspectiveCamera.h"
#include "CObject.h"
#include "CCube.h"
#include "CLight.h"

#include <iostream>
#include "IL\ilut.h"

#include "model.hpp"


CGame::CGame() : _input(*this), _camera(new CPerspectiveCamera()), _window(nullptr), _objList(), _lightList(), _alpha(0.0f), _flash(true)
{
    //ctor
    int ret = setup();
    if (ret) {
        std::cout << "Error" << std::endl;
        throw "error";
    }
}

CGame::~CGame()
{
    //dtor
}

int CGame::setup() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    _window = glfwCreateWindow(800, 600, "BasicStart", nullptr, nullptr);
    if (_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(_window);
    glfwSetFramebufferSizeCallback(_window, this->framebuffer_size_callback);
    glfwSetCursorPosCallback(_window, _input.mouse_callback);
    glfwSetScrollCallback(_window, _input.scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ilInit();
    iluInit();
    ilutInit();
    ilutEnable(ILUT_OPENGL_CONV);

    glEnable(GL_DEPTH_TEST);

    // RETRIEVE INFO
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    return 0;
}

void CGame::loop() {
    Shader shaderObj("G:/code/bidouille/WIP/shaders/vertexShader1.shader", "G:/code/bidouille/WIP/shaders/fragmentShader1.shader");
    Shader shaderLight("G:/code/bidouille/WIP/shaders/vertexShaderLight1.shader", "G:/code/bidouille/WIP/shaders/fragmentShaderLight1.shader");

    Model nanosuit("G:/code/bidouille/WIP/ressources/models/nanosuit/nanosuit.obj");
    //Model nanosuit("G:/code/bidouille/WIP/ressources/models/anvil.blend");
    //Model nanosuit("G:/code/bidouille/WIP/ressources/models/testChurros3/untitled2.fbx");

    std::cout << "Nano Loaded" << std::endl;

    Model churros("G:/code/bidouille/WIP/ressources/models/testChurros3/untitled2.obj");
    //Model churros("G:/code/bidouille/WIP/ressources/models/Minimal_Default.fbx");

    std::cout << "Churros Loaded" << std::endl;

    Model anvil("G:/code/bidouille/WIP/ressources/models/anvil/anvil.obj");

    IModel* cube = new CCube();
    IModel* light = new CLight();
//    CObject basic(*this, *cube, true);
    _lightList.push_back(new CObject(*this, *light, glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.5f), false, true));

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  -10.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int i = 0;
    for (glm::vec3& vec : cubePositions)
        _objList.push_back(new CObject(*this, *cube, vec, glm::vec3(1.0f), (i++ % 3 == 0)));

    _objList.push_back(new CObject(*this, nanosuit, glm::vec3(0.0f, -2.25f, 7.0f), glm::vec3(0.2f), true));
    _objList.push_back(new CObject(*this, churros, glm::vec3(-5.0f, -1.75f, 0.0f), glm::vec3(0.2f)));
    _objList.push_back(new CObject(*this, anvil, glm::vec3(5.0f, -1.75f, 0.0f), glm::vec3(0.2f)));

    //glm::vec3 cam = glm::vec3(0.0f, 0.0f, -3.0f);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while(!glfwWindowShouldClose(_window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        _input.processInput(deltaTime);

        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto obj : _lightList)
            obj->Draw(shaderLight);

        for (auto obj : _objList)
            obj->Draw(shaderObj);

        //_lightList[0]->SetPos(glm::vec3((float)sin(currentFrame)*10, 5.0f, (float)cos(currentFrame)*10));
        _lightList[0]->SetPos(glm::vec3((float)sin(currentFrame)*10, 5.0f, 5.0f));

        glfwSwapBuffers(_window);
        //glfwSetWindowShouldClose(_window, true);
        glfwPollEvents();
    }
    glfwTerminate();
}



void CGame::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void CGame::AddAlpha(float val)
{
    _alpha += val;
    if (_alpha >= 1.0f)
        _alpha = 1.0f;
    if (_alpha <= 0.0f)
        _alpha = 0.0f;
}
