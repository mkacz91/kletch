#ifndef KLETCH_DEMO_AIMER_DEMO_H
#define KLETCH_DEMO_AIMER_DEMO_H

#include "prefix.h"

#include "constrained_clothoid_demo.h"

namespace kletch {

class AimerDemo : public ConstrainedClothoidDemo
{
public:
    AimerDemo();

    virtual void render() override;
    virtual void handle_event(const DemoEvent& e) override;

protected:
    virtual void open() override;
    virtual void close() noexcept override;
};

} // namespace kletch

#endif // KLETCH_DEMO_AIMER_DEMO_H
