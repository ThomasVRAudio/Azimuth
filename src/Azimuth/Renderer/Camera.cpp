#include <Azimuth/Renderer/Camera.h>

namespace Azimuth
{

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 Camera::GetProjectionMatrix()
    {
        return glm::perspective(glm::radians(m_Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    }

    void Camera::UpdateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

    void Camera::ProcessMouseMovement(double xoffset, double yoffset)
    {
        print(xoffset << " " << yoffset);
        xoffset *= m_MouseSensitivity * Time::deltaTime();
        yoffset *= m_MouseSensitivity * Time::deltaTime();

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (true) // constrain pitch
        {

            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;
        }

        UpdateCameraVectors();
    }

    void Camera::ProcessMouseScroll(double xoffset, double yoffset)
    {
        m_Zoom -= (float)yoffset;
        if (m_Zoom < 1.0f)
            m_Zoom = 1.0f;
        if (m_Zoom > 45.0f)
            m_Zoom = 45.0f;
    }

    void Camera::ProcessKeyboard()
    {
        const float velocity = 2.5f * Time::deltaTime();

        if (Input::IsKeyPressed(W))
            Position += velocity * Front;

        if (Input::IsKeyPressed(S))
            Position -= velocity * Front;

        if (Input::IsKeyPressed(A))
            Position -= Right * velocity;

        if (Input::IsKeyPressed(D))
            Position += Right * velocity;

        if (Input::IsKeyPressed(Space))
            Position += Up * velocity;

        if (Input::IsKeyPressed(C))
            Position -= Up * velocity;
    }
}
