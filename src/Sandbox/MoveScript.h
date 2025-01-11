#include <Azimuth/Azimuth.h>

namespace Azimuth
{

    class MoveScript : public MonoScript
    {
    public:
        void OnStart();
        void OnUpdate();

    private:
        TransformComponent m_Transform;
    };
}