#include "event_bus.h"

EventBus& EventBus::GetInstance() {
  static EventBus instance;
  return instance;
}
