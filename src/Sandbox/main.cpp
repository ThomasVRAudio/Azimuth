#include <Azimuth/Azimuth.h>
#include <Sandbox.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Editor/EditorLayer.h>

int main()
{
    Azimuth::Application *app = new Azimuth::Application();

    EditorLayer *editorLayer = new EditorLayer();
    app->AddLayer(editorLayer);

    app->Run();

    delete app;
    delete editorLayer;

    return 0;
}