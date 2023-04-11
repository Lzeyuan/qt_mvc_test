#ifndef SINGLETON_MANAGER_H_
#define SINGLETON_MANAGER_H_
#include <any>
#include <map>
#include <memory>
/*
 * 仅用于存储对象
 */
class SingletonManager {
 private:
  SingletonManager() = default;
  ~SingletonManager() = default;
  SingletonManager(const SingletonManager &) = delete;
  SingletonManager &operator=(const SingletonManager &) = delete;
  SingletonManager(const SingletonManager &&) = delete;
  SingletonManager &operator=(const SingletonManager &&) = delete;

 public:
  static SingletonManager &GetInstance() {
    static SingletonManager singletonManager;
    return singletonManager;
  }

  template <typename T>
  void Register(std::shared_ptr<T> object) {
    std::any obj = object;
    size_t hash_code = typeid(T).hash_code();
    container_[hash_code] = obj;
  }

  template <typename T>
  void UnRegister() {
    size_t hash_code = typeid(T).hash_code();
    auto key = container_.find(hash_code);
    container_.erase(key);
  }

  template <typename T>
  std::shared_ptr<T> Get() {
    std::any object = container_[typeid(T).hash_code()];
    if (object.has_value()) {
      return std::any_cast<std::shared_ptr<T>>(object);
    } else {
      return nullptr;
    }
  }

 private:
  std::map<size_t, std::any> container_;
};

#endif  // SINGLETON_MANAGER_H_
