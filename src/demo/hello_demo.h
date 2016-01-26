#ifndef KLETCH_DEMO_HELLO_DEMO_H
#define KLETCH_DEMO_HELLO_DEMO_H

#include "prefix.h"

#include "demo.h"

namespace kletch {

class HelloDemo : public Demo
{
public:
    virtual void init() override;

    virtual void clean_up() override;

    virtual void render() override;

    virtual void handle_event(const DemoEvent& e) override;
};

} // namespace kletch

#endif // KLETCH_DEMO_HELLO_DEMO_H
