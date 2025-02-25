#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), double yaw = -90.0f, double pitch = 0.0f)
            : Position(position), WorldUp(up), m_Yaw(yaw), m_Pitch(pitch), Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(2.5f), m_MouseSensitivity(0.1f), m_Zoom(45.0f)
        {
            UpdateCameraVectors();
        }

        glm::mat4 GetViewMatrix();
        glm::mat4 GetProjectionMatrix();

        inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

        glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    protected:
        void UpdateCameraVectors();

        double m_Yaw;
        double m_Pitch;
        double m_MovementSpeed;
        double m_MouseSensitivity;
        double m_Zoom;

        float m_AspectRatio = 1.778f;
    };
}