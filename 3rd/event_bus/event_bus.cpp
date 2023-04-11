
#include "event_bus.h"

EventBus::EventBus()= default;

EventBus::~EventBus()= default;

EventBus* EventBus::instance = nullptr;
EventBus& EventBus::Get()
{
    if (!instance)
    {
        instance = new EventBus();
    }

    return *instance;
}
