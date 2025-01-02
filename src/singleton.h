#ifndef SINGLETON_H
#define SINGLETON_H

#include <fmt/core.h>

#include <memory>
#include <mutex>

template<typename T>
class Singleton
{
public:
  static T* getInstance()
  {
    static std::once_flag flag;
    std::call_once(flag, [&]() { m_instance = std::unique_ptr<T>(new T); });
    return m_instance.get();
  }

  ~Singleton() { fmt::println("Singleton destroyed"); }

protected:
  Singleton() = default;
  Singleton(const Singleton<T>&) = delete;
  Singleton& operator=(const Singleton<T>&) = delete;

  static std::unique_ptr<T> m_instance;
};

template<typename T>
std::unique_ptr<T> Singleton<T>::m_instance = nullptr;

#endif // SINGLETON_H