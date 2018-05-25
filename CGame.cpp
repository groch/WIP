#include "CGame.h"
#include "CPerspectiveCamera.h"
#include "CObject.h"
#include "CCube.h"
#include "CLight.h"
#include "CPlane.h"

#include <iostream>
#include "IL\ilut.h"
#include <glm/glm.hpp>

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
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
//    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    // RETRIEVE INFO
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    return 0;
}

void CGame::loop() {
    Shader shaderObj("G:/code/bidouille/WIP/shaders/vertexShader1.vert", "G:/code/bidouille/WIP/shaders/fragmentShader1.frag");
    Shader shaderObjTBN("G:/code/bidouille/WIP/shaders/vertexShader1.vert", "G:/code/bidouille/WIP/shaders/fragmentShaderTBN1.frag");
    Shader shaderObjTBN2("G:/code/bidouille/WIP/shaders/vertexShader1.vert", "G:/code/bidouille/WIP/shaders/fragmentShaderTBN2.frag");
    Shader shaderLight("G:/code/bidouille/WIP/shaders/vertexShaderLight1.vert", "G:/code/bidouille/WIP/shaders/fragmentShaderLight1.frag");
    Shader shaderStencil("G:/code/bidouille/WIP/shaders/vertexShaderLight1.vert", "G:/code/bidouille/WIP/shaders/SingleColorFragment.frag");
    Shader shaderGeoNorm("G:/code/bidouille/WIP/shaders/vertexShaderGeoNorm1.vert", "G:/code/bidouille/WIP/shaders/SingleColorFragment.frag", "G:/code/bidouille/WIP/shaders/geoNorm.geom");

    shaderGeoNorm.use();
    shaderGeoNorm.setVec3("color", 1.0f, 0.8125f, 0.0f);

    Model nanosuit("G:/code/bidouille/WIP/ressources/models/nanosuit/nanosuit.obj");
    //Model nanosuit("G:/code/bidouille/WIP/ressources/models/anvil.blend");
    //Model nanosuit("G:/code/bidouille/WIP/ressources/models/testChurros3/untitled2.fbx");

    std::cout << "Nano Loaded" << std::endl;

    Model churros("G:/code/bidouille/WIP/ressources/models/testChurros3/untitled2.obj");
    //Model churros("G:/code/bidouille/WIP/ressources/models/Minimal_Default.fbx");

    std::cout << "Churros Loaded" << std::endl;

    Model anvil("G:/code/bidouille/WIP/ressources/models/anvil/anvil.obj");
    Model african_head("G:/code/bidouille/WIP/ressources/models/african_head/african_head.obj");
    Model african_headT("G:/code/bidouille/WIP/ressources/models/african_head/african_headtangent.obj");

    std::cout << "Start Loading Car" << std::endl;

    Model car("G:/code/bidouille/WIP/ressources/models/car/Avent.obj");

    std::cout << "Car Loaded" << std::endl;

    IModel* cube = new CCube();
    IModel* light = new CLight();
//    CObject basic(*this, *cube, true);
    _lightList.push_back(new CObject(*this, *light, glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.5f), false, true));

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float secCounter = 0.0f;
    float lastFrame = 0.0f; // Time of last frame

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f, -10.0f),
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

    IModel* plane = new CPlane();
    IModel* plane2 = new CPlane(1);

    CObject planeObj(*this, *plane, glm::vec3( 0.0f, -11.0f,  0.0f), glm::vec3(10.0f));
    planeObj.ApplyRotation(glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

    CObject planeObj2(*this, *plane2, glm::vec3( 0.0f, -11.0f,  10.0f), glm::vec3(10.0f));
    planeObj2.ApplyRotation(glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

    CObject nanoObj(*this, nanosuit, glm::vec3(0.0f, -2.25f, 7.0f), glm::vec3(0.2f));
//    nanoObj.ApplyRotation(glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));

    //CObject nanoObj2(*this, nanosuit, glm::vec3(-3.0f, -2.25f, 7.0f), glm::vec3(0.2f));

    _objList.push_back(new CObject(*this, churros, glm::vec3(-5.0f, -1.75f, 0.0f), glm::vec3(0.2f)));

    CObject anvilObj(*this, anvil, glm::vec3(5.0f, -1.75f, 0.0f), glm::vec3(0.5f));
    CObject carObj(*this, car, glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.5f));

    glm::vec3 headPositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3(-2.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  0.0f,  0.0f),
        glm::vec3( 0.0f, -2.0f,  0.0f),
        glm::vec3( 0.0f,  2.0f,  0.0f),
        glm::vec3( 0.0f,  0.0f, -2.0f),
        glm::vec3( 0.0f,  0.0f,  2.0f)
    };


    //CObject head(*this, african_head, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.5f));
    CObject head(*this, african_headT, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.5f));

    std::vector<IObject*>   headTlist;
    for (glm::vec3& vec : headPositions)
        headTlist.push_back(new CObject(*this, african_headT, vec, glm::vec3(0.5f)));

    //head.ApplyRotation(glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));

    //glm::vec3 cam = glm::vec3(0.0f, 0.0f, -3.0f);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while(!glfwWindowShouldClose(_window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (currentFrame - secCounter >= 1.0f) {
            std::cout << "time to render this frame=" << deltaTime << "ms (~" << (1/deltaTime) << "FPS)" << std::endl;
            secCounter = currentFrame;
        }


        _input.processInput(deltaTime);

        //_lightList[0]->SetPos(glm::vec3((float)sin(currentFrame)*10, 5.0f, (float)cos(currentFrame)*10));
        _lightList[0]->SetPos(glm::vec3((float)sin(currentFrame)*10, 0.0f, 5.0f));

        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //glStencilMask(0x00);

        for (auto obj : _lightList)
            obj->Draw(shaderLight, glm::vec3(1.0f));

//        glStencilFunc(GL_ALWAYS, 1, 0xFF);
//        glStencilMask(0xFF);

        for (auto obj : _objList)
            obj->Draw(shaderObj, glm::vec3(1.0f));

        head.Draw(shaderObjTBN2, glm::vec3(1.0f));
//        head.Draw(shaderGeoNorm, glm::vec3(1.0f));

        for (auto obj : headTlist)
            obj->Draw(shaderObjTBN, glm::vec3(1.0f));


        anvilObj.Draw(shaderObjTBN, glm::vec3(1.0f));
        nanoObj.Draw(shaderObj, glm::vec3(1.0f));

        //nanoObj2.Draw(shaderObjTBN, glm::vec3(1.0f));

        carObj.Draw(shaderObj, glm::vec3(1.0f));
        planeObj.Draw(shaderObjTBN2, glm::vec3(1.0f));
        planeObj2.Draw(shaderObjTBN2, glm::vec3(1.0f));

        //anvilObj.Draw(shaderGeoNorm, glm::vec3(1.0f));
        //nanoObj.Draw(shaderGeoNorm, glm::vec3(1.0f));

//        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//        glStencilMask(0x00);
//        glDisable(GL_DEPTH_TEST);

//        for (auto obj : _objList)
//            obj->Draw(shaderStencil, glm::vec3(1.1f));
//
//        glStencilMask(0xFF);
//        glEnable(GL_DEPTH_TEST);


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
    std::cout << "Alpha=" << _alpha << std::endl;
}
