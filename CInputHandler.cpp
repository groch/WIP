#include "CInputHandler.h"
#include "CGame.h"

CGame* CInputHandler::_game = nullptr;
float CInputHandler::_lastX = 0.0f;
float CInputHandler::_lastY = 0.0f;
bool CInputHandler::_firstMove = true;

CInputHandler::CInputHandler(CGame& game)
{
    //ctor
    _game = &game;
}

CInputHandler::~CInputHandler()
{
    //dtor
}

int CInputHandler::processInput(float deltaTime)
{
    ICamera& cam = _game->GetCamera();
    GLFWwindow* window = _game->GetWindow();
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        return 1;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        return -1;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(ICamera::FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(ICamera::BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(ICamera::LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(ICamera::RIGHT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.ProcessKeyboard(ICamera::UP, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        cam.ProcessKeyboard(ICamera::DOWN, deltaTime);
    return 0;
}

void CInputHandler::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (_firstMove) {
        _lastX = xpos;
        _lastY = ypos;
        _firstMove = false;
    }
    float xoffset = xpos - _lastX;
    float yoffset = _lastY - ypos; // reversed since y-coordinates range from bottom to top
    _lastX = xpos;
    _lastY = ypos;
    if (_game != nullptr) {
        //std::cout << "mouse_callback:xoffset=" << xoffset << ", yoffset=" << yoffset << std::endl;
        _game->GetCamera().ProcessMouseMovement(xoffset, yoffset);
    }
}

void CInputHandler::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (_game != nullptr) {
        //std::cout << "mouse_callback:xoffset=" << xoffset << ", yoffset=" << yoffset << std::endl;
        _game->GetCamera().ProcessMouseScroll(yoffset);
    }
}
