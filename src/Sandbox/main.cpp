#include <Azimuth/Azimuth.h>
#include <Sandbox.h>
#include <MoveScript.h>

int main()
{
    Azimuth::Application *app = new Azimuth::Application();

    Entity entity = app->MainScene->ECS.CreateEntity();

    ScriptsComponent objectScripts;
    std::shared_ptr<MoveScript> moveScript = std::make_shared<MoveScript>();

    moveScript->SetScene(app->MainScene);
    moveScript->SetEntity(entity);

    objectScripts.AddScript(std::static_pointer_cast<MonoScript>(moveScript));
    app->MainScene->ECS.AddComponent<ScriptsComponent>(entity, std::move(objectScripts));

    app->Run();

    delete app;

    return 0;
}