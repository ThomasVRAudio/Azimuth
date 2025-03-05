#include <Azimuth/Core/Application.h>

using namespace Azimuth;

int main()
{
    Application *app = new Azimuth::Application();
    app->Run();

    delete app;
    return 0;
}