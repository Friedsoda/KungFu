#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
    Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
    ~Camera();
    
    // camera attributes
    glm::vec3 Position;
    glm::vec3 Forward;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;
    // euler angles
    float Pitch;
    float Yaw;
    // camera options
    float KeySensitivity = 0.1;
    float MouseSensitivity = 0.001;
    float speedZ = 0;
    float speedX = 0;
    float speedY = 0;
    bool ifMoving;
    
    glm::mat4 GetViewMatrix();
    void ProcessMouseMovement(float xOffset, float yOffset);
    void UpdateCameraPos();
    void MoveCamera(float step);
    
private:
    void UpdateCameraVectors();
};