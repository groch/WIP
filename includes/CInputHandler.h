#ifndef CINPUTHANDLER_H
#define CINPUTHANDLER_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>

class CGame;

class CInputHandler
{
    public:
        CInputHandler(CGame& game);
        ~CInputHandler();

        int             processInput(float);
        static void     mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void     scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    protected:

    private:
        static CGame*               _game;
        static float                _lastX, _lastY;
        static bool                 _firstMove;
};

#endif // CINPUTHANDLER_H
