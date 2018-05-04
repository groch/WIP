#ifndef CGAME_H
#define CGAME_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <vector>

#include "IObject.h"
#include "ICamera.h"
#include "CInputHandler.h"

class CGame
{
    public:
        CGame();
        ~CGame();

        ICamera&                GetCamera() { return *_camera; }
        GLFWwindow*             GetWindow() { return _window; }
        std::vector<IObject*>   GetObjList() { return _objList; }
        std::vector<IObject*>   GetLightList() { return _lightList; }
        float                   GetAlpha() { return _alpha; }
        void                    AddAlpha(float);

        void                    loop();

    protected:

    private:
        int                     setup();
        static void             framebuffer_size_callback(GLFWwindow*, int, int);

        CInputHandler           _input;
        ICamera*                _camera;
        GLFWwindow*             _window;
        std::vector<IObject*>   _objList;
        std::vector<IObject*>   _lightList;
        float                   _alpha;
};

#endif // CGAME_H
