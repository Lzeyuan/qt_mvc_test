#pragma once
#include <algorithm>
#include <any>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>

#include "SignalSlot.h"

class EventBus {
 public:
  using SignalVoid = muduo::Signal<void(std::any)>;
  using SignalMap = std::map<std::string, SignalVoid>;
  using Slot = muduo::Slot;

  // 持有就是监听，释放就是注销
  template <typename TMessage, typename EventHandler>
  Slot Subscribe(const std::string& token, EventHandler& handler) {
    auto signal_void_it = map_.find(token);
    if (map_.end() != signal_void_it) {
      Slot slot =
          (*signal_void_it).second.connect([handler](const auto& message) {
            handler(std::any_cast<TMessage>(message));
          });
      return slot;
    }
    auto signal_void = SignalVoid{};
    map_.emplace(token, signal_void);
    return signal_void.connect([handler](const auto& message) {
      handler(std::any_cast<TMessage>(message));
    });
  }

  template <typename TMessage>
  void Publish(const std::string& token, TMessage& message) {
    // std::unique_lock<std::mutex> locker(pub_mtx_);
    auto signal_void_it = map_.find(token);
    if (map_.end() != signal_void_it) {
      (*signal_void_it).second.call(message);
    }
  }

 private:
  SignalMap map_;

#pragma region Singleton
 public:
  static EventBus& GetInstance();

 private:
  EventBus() = default;
  ~EventBus() = default;
  EventBus(const EventBus&) = delete;
  EventBus& operator=(const EventBus&) = delete;
  EventBus(const EventBus&&) = delete;
  EventBus& operator=(const EventBus&&) = delete;
#pragma endregion
};
