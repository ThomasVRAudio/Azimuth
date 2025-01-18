#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Editor/ImGuiStyling.h>
#include <Azimuth/Renderer/Window.h>

namespace Azimuth
{

    class EditorUI
    {
    public:
        static void Init();
        static void DrawUI();
        static void DrawEditorScene(unsigned int *texture);

        template <typename DrawFunction>
        static void DrawToBuffer(unsigned int *framebuffer, DrawFunction drawFunction)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
            int width, height;
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            drawFunction();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        static void EndDraw();
        static void CreateDocker();
        inline static ImVec2 GetSceneWindowPos() { return m_SceneWindowPos; }
        inline static ImVec2 GetSceneWindowSize() { return m_SceneWindowSize; }

    private:
        static ImGuiIO *io;
        static ImGuiWindowFlags m_WindowFlags;
        static ImVec2 m_SceneWindowPos;
        static ImVec2 m_SceneWindowSize;
    };

}