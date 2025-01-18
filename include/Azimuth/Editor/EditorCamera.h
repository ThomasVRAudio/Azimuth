#pragma once
#include <Azimuth/Renderer/Camera.h>
#include <Azimuth/Events/MouseEvents.h>

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

    struct WindowOffset
    {
        float x;
        float y;
    };

    class EditorCamera : public Camera
    {
    public:
        EditorCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f)
            : Camera(position, up, yaw, pitch)
        {

            g_ScrollEvent.Attach(this, &ProcessMouseScroll);
            g_CursorEvent.Attach(this, &ProcessMouseMovement);
        }

        void SetEditorSceneCenter(WindowOffset offset, WindowOffset size);
        void ProcessKeyboard();
        void ProcessMouseMovement(double xpos, double ypos);
        void ProcessMouseScroll(double xoffset, double yoffset);

    private:
        double lastX, lastY;
        bool firstMouse = true;

        WindowOffset m_WindowOffset = {0.0f, 0.0f};
    };

}