#ifndef ICAMERA_H
#define ICAMERA_H

#include <glm/glm.hpp>
#include <glad/glad.h>

class ICamera
{
    public:
        enum Camera_Movement {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT
        };
        virtual ~ICamera() = default;

        virtual glm::mat4   GetViewMatrix() const = 0;
        virtual glm::mat4   GetProjMatrix() const = 0;
        virtual glm::vec3   GetPos() = 0;

        virtual void        ProcessKeyboard(Camera_Movement, float) = 0;
        virtual void        ProcessMouseMovement(float, float, GLboolean constrainPitch = true) = 0;
        virtual void        ProcessMouseScroll(float) = 0;

    protected:

    private:
};

#endif // ICAMERA_H
