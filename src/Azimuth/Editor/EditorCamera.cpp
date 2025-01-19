#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorCamera.h>

namespace Azimuth
{
    void EditorCamera::ProcessMouseMovement(double xpos, double ypos)
    {

        if (!Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            firstMouse = true;
            return;
        }

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

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

    void EditorCamera::ProcessMouseScroll(double xoffset, double yoffset)
    {
        m_Zoom -= (float)yoffset;
        if (m_Zoom < 1.0f)
            m_Zoom = 1.0f;
        if (m_Zoom > 45.0f)
            m_Zoom = 45.0f;
    }

    void EditorCamera::ProcessKeyboard()
    {
        const float velocity = 2.5f * Time::deltaTime();

        if (!Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) // rather check if mouse in window
        {
            firstMouse = true;
            return;
        }

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

        if (Input::IsKeyPressed(LeftShift) || Input::IsKeyPressed(RightShift))
            Position -= Up * velocity;
    }

    void EditorCamera::SetEditorSceneCenter(WindowOffset offset, WindowOffset size)
    {
        float height = offset.y + size.y / 2.0f;
        float width = offset.x + size.x / 2.0f;
        m_WindowOffset = {width, height};
    }

}

#endif