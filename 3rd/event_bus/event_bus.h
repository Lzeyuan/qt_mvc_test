#pragma once
#include <QObject>
#include <any>
#include <functional>
#include <map>
#include <mutex>
#include <string>

#include "signal_slot.h"

class EventBus : QObject {
  Q_OBJECT
 public:
  using SignalVoid = muduo::Signal<void(std::any)>;
  using SignalMap = std::map<std::string, SignalVoid>;
  using Slot = muduo::Slot;

    // 持有就是监听，释放就是注销
  template <typename Token, typename TMessage, typename EventHandler>
  Slot Subscribe(EventHandler& handler) {
    return Subscribe<TMessage>(typeid(Token).name(), handler);
  }

  // 持有就是监听，释放就是注销
  template <typename TMessage, typename EventHandler>
  Slot Subscribe(const std::string& token, EventHandler& handler) {
    auto func = [this, handler](const auto& message) {
      QMetaObject::invokeMethod(
          this,
          [handler, message]() { handler(std::any_cast<TMessage>(message)); },
          Qt::QueuedConnection);
    };

    auto signal_void_it = map_.find(token);
    if (map_.end() != signal_void_it) {
      Slot slot = (*signal_void_it).second.connect(func);
      return slot;
    }
    auto signal_void = SignalVoid{};
    {
      std::lock_guard lock(subscribe_mutex_);
      map_.emplace(token, signal_void);
    }

    return signal_void.connect(func);
  }

  template <typename Token, typename TMessage>
  void Publish(TMessage& message) {
    Publish(typeid(Token).name(), message);
  }

  template <typename TMessage>
  void Publish(const std::string& token, TMessage& message) {
    auto signal_void_it = map_.find(token);
    if (map_.end() != signal_void_it) {
      (*signal_void_it).second.call(message);
    }
  }

 private:
  std::mutex subscribe_mutex_;
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
