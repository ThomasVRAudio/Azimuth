#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Events/MouseEvents.h>
#include <Azimuth/Core/Time.h>
#include <Azimuth/Core/Input.h>

namespace Azimuth
{
    enum Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UPWARD,
        DOWNWARD
    };

    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f)
            : Position(position), WorldUp(up), m_Yaw(yaw), m_Pitch(pitch), Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(2.5f), m_MouseSensitivity(0.1f), m_Zoom(45.0f)
        {
            g_ScrollEvent.Attach(this, &ProcessMouseScroll);
            g_CursorEvent.Attach(this, &ProcessMouseMovement);

            UpdateCameraVectors();
        }
        glm::mat4 GetViewMatrix();
        glm::mat4 GetProjectionMatrix();
        void ProcessKeyboard();
        void ProcessMouseMovement(double xoffset, double yoffset);
        void ProcessMouseScroll(double xoffset, double yoffset);
        inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
        glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        bool firstMouse = true;
        float lastX = 800.0f / 2.0;
        float lastY = 600.0 / 2.0;

    protected:
        void UpdateCameraVectors();

        float m_Yaw;
        float m_Pitch;
        float m_MovementSpeed;
        float m_MouseSensitivity;
        float m_Zoom;

        float m_AspectRatio = 1.778f;
    };
}