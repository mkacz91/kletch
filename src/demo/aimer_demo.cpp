#include "aimer_demo.h"
#include <vector>

namespace kletch {

AimerDemo::AimerDemo() : ConstrainedClothoidDemo("Clothoid Aim")
{ }

void AimerDemo::render()
{
    ConstrainedClothoidDemo::render();
}

void AimerDemo::handle_event(const DemoEvent& e)
{
    ConstrainedClothoidDemo::handle_event(e);
    if (e.handled())
    {
        //m_aim_result = m_aimer.aim(m_origin, m_target)
        return;

    }
}

void AimerDemo::open()
{
    ConstrainedClothoidDemo::open();
}

void AimerDemo::close() noexcept
{
    ConstrainedClothoidDemo::close();
}

} // namespace kletch
