#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Editor/EditorManager.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/System/Files.h>
#include <Azimuth/Editor/EditorFilepicker.h>

namespace Azimuth
{
    class EditorFilepicker;
    class EditorPropertiesPanel
    {

    public:
        static void DrawPanel();

    private:
        static void DrawVec3Box(glm::vec3 &vec3, std::string name, const std::array<std::string, 3> &labels, float speed = 0.01f);
        static void AddComponent();
        static void DirectoryCombo(const std::string &path, std::string &currentItem, MaterialComponent &material);
        static EditorFilepicker m_Filepicker;

        enum Filepicker
        {
            MESH_PICKER,
            TEXTURE_PICKER
        };
        inline static Filepicker m_SelectedFilepicker = MESH_PICKER;
        inline static std::string m_SelectedTextureName;
        inline static unsigned int m_SelectedTextureSlot;
    };
}

#endif