#include <Azimuth/Azimuth.h>
#include <PlayerMovement.h>

using namespace Azimuth;

int main()
{
    Application *app = new Azimuth::Application();
    Scene *scene = app->ActiveScene;

    Entity player = scene->CreateEntity("Player");
    Entity otherEntity = scene->CreateEntity("Meshless Entity");

    scene->AddScript(player, std::make_shared<PlayerMovement>());

    app->Run();

    delete app;
    return 0;
}