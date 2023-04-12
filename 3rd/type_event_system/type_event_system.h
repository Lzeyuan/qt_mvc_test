#pragma once

#include <QDebug>
#include <QObject>
#include <algorithm>
#include <any>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>

class Observer {
 public:
  Observer(size_t param_type_hash_code, std::function<void(std::any)>&& func);
  ~Observer() = default;

  std::function<void(std::any)> func_ = nullptr;
  size_t param_type_hash_code_ = 0;
};

class TypeEventSystem : public QObject {
  Q_OBJECT
 public:
  using UnorderedMultimap = std::unordered_multimap<size_t, Observer>;

  template <typename EventType, typename ParamType, typename EventHandler>
  void Subscribe(EventHandler& handler) {
    std::unique_lock<std::mutex> locker(sub_mtx_);
    Observer observer{typeid(ParamType).hash_code(),
                      [handler](const auto& message) {
                        handler(std::any_cast<ParamType>(message));
                      }};
    subscribers_.emplace(typeid(EventType).hash_code(), observer);
  }

  template <typename EventType, typename ParamType, typename EventHandler>
  void UnSubscribe(EventHandler& handler) {}

  template <typename EventType, typename ParamType>
  void Publish(const ParamType& message) {
    Publish<EventType>(message, Qt::AutoConnection);
  }

  template <typename EventType, typename ParamType>
  void Publish(const ParamType& message, Qt::ConnectionType connection_type) {
    std::unique_lock<std::mutex> locker(pub_mtx_);
    size_t token = typeid(EventType).hash_code();
    const auto range = subscribers_.equal_range(token);
    std::for_each(
        range.first, range.second,
        [this, &message, connection_type](UnorderedMultimap::value_type& vt) {
          std::function<void(std::any)> func = vt.second.func_;
          QMetaObject::invokeMethod(
              this, [message, func, connection_type]() { func(message); },
              connection_type);
        });
  }

 protected:
  UnorderedMultimap subscribers_;
  std::mutex pub_mtx_;
  std::mutex sub_mtx_;

#pragma region Singleton
 public:
  static TypeEventSystem& Get();

 private:
  TypeEventSystem() = default;
  ~TypeEventSystem() = default;
  TypeEventSystem(const TypeEventSystem&) = delete;
  TypeEventSystem& operator=(const TypeEventSystem&) = delete;
  TypeEventSystem(const TypeEventSystem&&) = delete;
  TypeEventSystem& operator=(const TypeEventSystem&&) = delete;
#pragma endregion
};
