#include "Camera.h"

Camera::Camera()
{
    Position = glm::vec3(0, 1.3f, 9.0f);
    WorldUp = glm::vec3(0, 1.0f, 0);
    Pitch = glm::radians(-7.0f);
    Yaw = glm::radians(180.0f);
    ifMoving = false;
    UpdateCameraVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
    Position = position;
    WorldUp = worldup;
    Forward = glm::normalize(target - position);
    Right = glm::normalize(glm::cross(Forward, WorldUp));
    Up = glm::normalize(glm::cross(Right, Forward));
    ifMoving = false;
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
    Position = position;
    WorldUp = worldup;
    Pitch = pitch;
    Yaw = yaw;
    ifMoving = false;
    UpdateCameraVectors();
}

Camera::~Camera()
{
    ;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Forward, WorldUp);
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
    Pitch += yOffset * MouseSensitivity;
    Yaw += xOffset * MouseSensitivity;
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
    Forward.y = glm::sin(Pitch);
    Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
    Right = glm::normalize(glm::cross(Forward, WorldUp));
    Up = glm::normalize(glm::cross(Forward, Right));
}

void Camera::UpdateCameraPos()
{
    Position += (Forward * speedZ + Right * speedX + Up * speedY) * KeySensitivity;
}

void Camera::MoveCamera(float step)
{
    Position.x += step;
}