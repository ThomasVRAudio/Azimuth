#include <Azimuth/Azimuth.h>
#include <PlayerMovement.h>

using namespace Azimuth;

int main()
{
    Application *app = new Azimuth::Application();
    Scene *scene = app->ActiveScene;

    Entity entity = scene->CreateObject();

    scene->AddScript(entity, std::make_shared<PlayerMovement>());

    app->Run();

    delete app;
    return 0;
}