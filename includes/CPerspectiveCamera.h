#ifndef CPERSPECTIVECAMERA_H
#define CPERSPECTIVECAMERA_H

#include "ICamera.h"

class CPerspectiveCamera : public ICamera
{
    public:
        // Default camera values
        static constexpr float  YAW         = -90.0f;
        static constexpr float  PITCH       =  0.0f;
        const float             SPEED       =  2.5f;
        const float             SENSITIVITY =  0.1f;
        const float             ZOOM        =  60.0f;

                                CPerspectiveCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f),
                                                    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                                                    float yaw = YAW,
                                                    float pitch = PITCH);
        virtual                 ~CPerspectiveCamera();
                                CPerspectiveCamera(const CPerspectiveCamera& other);

        virtual glm::mat4       GetViewMatrix() const;
        virtual glm::mat4       GetProjMatrix() const;
        virtual glm::vec3       GetPos() { return _position; }

        glm::vec3               GetPosition() const { return _position; }
        void                    SetPosition(glm::vec3 val) { _position = val; }
        void                    AddPosition(glm::vec3 val) { _position += val; }
        glm::vec3               GetDirection() const { return _direction; }
        void                    SetDirection(glm::vec3 val) { _direction = val; }
        glm::vec3               GetScale() const { return _scale; }
        void                    SetScale(glm::vec3 val) { _scale = val; }
        glm::vec3               GetUp() const { return _up; }
        void                    SetUp(glm::vec3 val) { _up = val; }
        float                   GetFoV() const { return _fov; }
        void                    SetFoV(float val) { _fov = val; }

        void                    SetYawPitch(float, float);
        glm::mat4               LookAt(glm::vec3, glm::vec3, glm::vec3) const;


        virtual void            ProcessKeyboard(Camera_Movement, float);
        virtual void            ProcessMouseMovement(float, float, GLboolean constrainPitch = true);
        virtual void            ProcessMouseScroll(float);

    protected:

    private:
        void                    ComputeMatrix();
        void                    updateCameraVectors();

        glm::vec3               _position;
        glm::vec3               _direction; //front
        glm::vec3               _up;
        glm::vec3               _right;
        glm::vec3               _worldUp;
        glm::vec3               _scale;

        float                   _yaw;
        float                   _pitch;
        float                   _mvSpeed;
        float                   _mouseSensitivity;
        float                   _fov;

        const glm::vec3         _noScale = glm::vec3(1.0f);
};

#endif // CPERSPECTIVECAMERA_H
