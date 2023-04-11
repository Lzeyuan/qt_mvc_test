
#include "event_bus.h"

//Observer::Observer(std::weak_ptr<QObject> object,
//                   std::function<void(std::any)> &&func)
//    : object_(object), func_(func) {
//
//}

EventBus& EventBus::Get() {
  static EventBus instance;
  return instance;
}
