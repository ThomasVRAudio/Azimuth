#include <Azimuth/Renderer/Camera.h>

namespace Azimuth
{

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 Camera::GetProjectionMatrix()
    {
        return glm::perspective(glm::radians(static_cast<float>(m_Zoom)), m_AspectRatio, 0.1f, 100.0f);
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

}
