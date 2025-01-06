#include <Vengine/Vengine.h>
#include <Sandbox.h>

int main()
{
    Vengine::Application *app = new Vengine::Application();

    Sandbox *sandbox = new Sandbox();
    app->AddLayer(sandbox);

    app->Run();

    delete app;

    return 0;
}