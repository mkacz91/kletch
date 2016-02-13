#ifndef KLETCH_DEMO_AIMER_DEMO_H
#define KLETCH_DEMO_AIMER_DEMO_H

#include "prefix.h"

#include "camera2.h"
#include "control_overlay.h"
#include "demo.h"

namespace kletch {

class AimerDemo : public Demo
{
public:
    AimerDemo();

    virtual void render() override;
    virtual void handle_event(const DemoEvent& e) override;

protected:
    virtual void open() override;
    virtual void close() noexcept override;

private:
    Camera2 m_camera;
    ControlOverlay m_control_overlay;

    vec2f m_origin = 0;
    vec2f m_target = vec2f(2, 2);
    vec2f m_tangent_tip = vec2f(1, 0);
};

} // namespace kletch

#endif // KLETCH_DEMO_AIMER_DEMO_H
