#include "CPerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include <iostream>

CPerspectiveCamera::CPerspectiveCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
                                                                _position(position),
                                                                _direction(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                                _up(up),
                                                                _worldUp(glm::normalize(up)),
                                                                _scale(glm::vec3(1.0f)),
                                                                _yaw(yaw),
                                                                _pitch(pitch),
                                                                _mvSpeed(CPerspectiveCamera::SPEED),
                                                                _mouseSensitivity(CPerspectiveCamera::SENSITIVITY),
                                                                _fov(CPerspectiveCamera::ZOOM)
{
    updateCameraVectors();
}

CPerspectiveCamera::~CPerspectiveCamera()
{
    //dtor
}

CPerspectiveCamera::CPerspectiveCamera(const CPerspectiveCamera& other) :
                                                                _position(other._position),
                                                                _direction(other._direction),
                                                                _up(other._up),
                                                                _worldUp(other._worldUp),
                                                                _scale(other._scale),
                                                                _yaw(other._yaw),
                                                                _pitch(other._pitch),
                                                                _mvSpeed(other._mvSpeed),
                                                                _mouseSensitivity(other._mouseSensitivity),
                                                                _fov(other._fov)
{
    //copy ctor
    updateCameraVectors();
}

glm::mat4 CPerspectiveCamera::GetViewMatrix() const {
    //glm::mat4 viewMat = glm::lookAt(_position, _position + _direction, _up);
    glm::mat4 viewMat = LookAt(_position, _position + _direction, _up);
    if (_scale != _noScale)
        viewMat = glm::scale(viewMat, _scale);
    return viewMat;
}

glm::mat4 CPerspectiveCamera::GetProjMatrix() const {
    //glm::mat4 viewMat = glm::lookAt(_position, _position + _direction, _up);
    glm::mat4 mat = glm::perspective(glm::radians(_fov), 16.0f / 9.0f, 0.1f, 100.0f);
    //glm::mat4 mat = glm::ortho(-800.0f, 800.0f, -450.0f, 450.0f, 0.001f, 3000.0f);
    return mat;
}

glm::mat4 CPerspectiveCamera::LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp) const {
    glm::vec3 dir = glm::normalize(pos - target);
    glm::vec3 right = glm::normalize(glm::cross(glm::normalize(worldUp), dir));
    glm::vec3 up = glm::cross(dir, right);

    // COLUMN MAJOR !!!
    glm::mat4 viewMat = glm::mat4(glm::vec4(right.x, up.x, dir.x, 0.0f),
                                  glm::vec4(right.y, up.y, dir.y, 0.0f),
                                  glm::vec4(right.z, up.z, dir.z, 0.0f),
                                  glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

//    glm::mat4 viewMat = glm::mat4(1.0f);
//    viewMat[0][0] = right.x;
//    viewMat[1][0] = right.y;
//    viewMat[2][0] = right.z;
//    viewMat[0][1] = up.x;
//    viewMat[1][1] = up.y;
//    viewMat[2][1] = up.z;
//    viewMat[0][2] = dir.x;
//    viewMat[1][2] = dir.y;
//    viewMat[2][2] = dir.z;

//    _position = pos;
//    _direction = dir;
//    _up = up;
//    updateCameraVectors();

    // COLUMN MAJOR !!!
    glm::mat4 posMat = glm::mat4(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                                 glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                                 glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
                                 glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f));

//    glm::mat4 posMat = glm::mat4(1.0f);
//    posMat[3][0] = -pos.x;
//    posMat[3][1] = -pos.y;
//    posMat[3][2] = -pos.z;

//    for (int i = 0; i < 4 ; i++) {
//        for (int j = 0; j < 4 ; j++) {
//            std::cout << "posMat["<<i<<"]["<<j<<"]="<<posMat[i][j]<<std::endl;
//        }
//    }
//    std::cout << std::endl;

    return viewMat * posMat;
}

// TEST ?
void CPerspectiveCamera::ComputeMatrix() {
    //glm::mat4 mat = _viewMatrix;
    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::translate(mat, _position);

    _direction.x = cos(glm::radians(_pitch)) * cos(glm::radians(_yaw));
    _direction.y = sin(glm::radians(_pitch));
    _direction.z = cos(glm::radians(_pitch)) * sin(glm::radians(_yaw));

    if (_yaw != 0.0f || _pitch != 0.0f)
        mat = glm::rotate(mat, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5, 1.0, 0.0));
    if (_scale != _noScale)
        mat = glm::scale(mat, _scale);
    //_viewMatrix = mat;
}

void CPerspectiveCamera::updateCameraVectors() {
    glm::vec3 fronta;
    fronta.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    fronta.y = sin(glm::radians(_pitch));
    fronta.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _direction = glm::normalize(fronta);
    // Also re-calculate the Right and Up vector
    _right = glm::normalize(glm::cross(_direction, _worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    _up = glm::normalize(glm::cross(_right, _direction));
}

void CPerspectiveCamera::SetYawPitch(float yaw, float pitch) {
    _yaw = yaw;
    _pitch = pitch;
}

void CPerspectiveCamera::ProcessKeyboard(ICamera::Camera_Movement direction, float deltaTime)
{
    float velocity = _mvSpeed * deltaTime;
    if (direction == CPerspectiveCamera::FORWARD)
        _position += _direction * velocity;
    if (direction == CPerspectiveCamera::BACKWARD)
        _position -= _direction * velocity;
    if (direction == CPerspectiveCamera::LEFT)
        _position -= _right * velocity;
    if (direction == CPerspectiveCamera::RIGHT)
        _position += _right * velocity;
    //_position.y = 0.0f;
}

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void CPerspectiveCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= _mouseSensitivity;
    yoffset *= _mouseSensitivity;

    _yaw   += xoffset;
    _pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;
    }

    //std::cout << "ProcessMouseMovement: _yaw=" << _yaw << ", _pitch=" << _pitch << std::endl;
    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void CPerspectiveCamera::ProcessMouseScroll(float yoffset)
{
    if (_fov >= 1.0f && _fov <= ZOOM)
        _fov -= yoffset;
    if (_fov <= 1.0f)
        _fov = 1.0f;
    if (_fov >= ZOOM)
        _fov = ZOOM;
    //std::cout << "scroll,yoffset="<<yoffset<<"fov=" << _fov << std::endl;
}
