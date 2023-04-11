#pragma once
#include <QObject>
#include <algorithm>
#include <any>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>

class IEvent {};

class Observer {
 public:
  explicit Observer() = default;
  // Observer(std::weak_ptr<QObject> object, std::function<void(std::any)>&&
  // func);
  ~Observer() = default;

  QObject* object_;
  std::function<void(std::any)> func_;
};

class EventBus {
 public:
  using UnorderedMultimap = std::unordered_multimap<int, Observer>;

  template <typename TMessage, typename EventHandler>
  void Subscribe(const int& token, QObject* object, EventHandler& handler) {
    std::unique_lock<std::mutex> locker(sub_mtx_);
    Observer observer;
    observer.object_ = object;
    observer.func_ = [func =
                          std::forward<EventHandler>(handler)](auto message) {
      func(std::any_cast<TMessage>(message));
    };

    subscribers_.emplace(token, observer);
  }

  template <typename TMessage>
  void Publish(const int& token, const TMessage& message) {
    std::unique_lock<std::mutex> locker(pub_mtx_);
    const auto range = subscribers_.equal_range(token);
    std::for_each(range.first, range.second,
                  [message](UnorderedMultimap::value_type& vt) {
                    QMetaObject::invokeMethod(
                        vt.second.object_,
                        [message, vt]() {
                          vt.second.func_(std::any_cast<TMessage>(message));
                        },
                        Qt::QueuedConnection);
                  });
  }

 protected:
  UnorderedMultimap subscribers_;
  std::mutex pub_mtx_;
  std::mutex sub_mtx_;

#pragma region Singleton
 public:
  static EventBus& Get();

 private:
  EventBus() = default;
  ~EventBus() = default;
  EventBus(const EventBus&) = delete;
  EventBus& operator=(const EventBus&) = delete;
  EventBus(const EventBus&&) = delete;
  EventBus& operator=(const EventBus&&) = delete;
#pragma endregion
};
