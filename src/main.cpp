#include <Vengine/Vengine.h>

int main()
{

    Vengine::Application *app = new Vengine::Application();
    app->Run();
    delete app;

    return 0;
}