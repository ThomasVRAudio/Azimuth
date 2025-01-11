#include <Azimuth/Azimuth.h>
#include <MoveScript.h>

using namespace Azimuth;

int main()
{
    Application *app = new Azimuth::Application();
    Scene *scene = app->ActiveScene;

    Entity entity = scene->CreateObject();

    auto moveScript = std::make_shared<MoveScript>();
    scene->AddScript(entity, std::static_pointer_cast<MonoScript>(std::move(moveScript)));

    app->Run();

    delete app;
    return 0;
}