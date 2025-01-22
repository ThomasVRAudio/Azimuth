#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Editor/ImGuiStyling.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Editor/EditorPropertiesPanel.h>
#include <Azimuth/Editor/EditorHierarchyPanel.h>

namespace Azimuth
{
    class Scene;

    class EditorUI
    {
    public:
        static void Init(Scene *scene);
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
        static void SetAspectConstraints(ImGuiSizeCallbackData *data);
        inline static ImGuiIO *io = nullptr;
        inline static ImGuiWindowFlags m_WindowFlags;
        inline static ImVec2 m_SceneWindowPos;
        inline static ImVec2 m_SceneWindowSize;
        inline static float m_SceneWindowAspectRatio = 1.778f;
        inline static Scene *m_Scene = nullptr;
        inline static Entity m_SelectedEntity;
        friend class EditorHierarchyPanel;
        friend class EditorPropertiesPanel;
    };

}

#endif