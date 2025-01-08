#include <Azimuth/Azimuth.h>
#include <Sandbox.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Scene/Scene.h>

int main()
{
    Azimuth::Application *app = new Azimuth::Application();

    Sandbox *sandbox = new Sandbox();

    app->AddLayer(sandbox);

    app->Run();

    delete app;

    return 0;
}