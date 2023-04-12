
#include "type_event_system.h"

Observer::Observer(size_t param_type_hash_code,
                   std::function<void(std::any)> &&func)
    : param_type_hash_code_(param_type_hash_code), func_(func) {}

TypeEventSystem &TypeEventSystem::Get() {
  static TypeEventSystem instance;
  return instance;
}
