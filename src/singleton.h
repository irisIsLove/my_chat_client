#ifndef SINGLETON_H
#define SINGLETON_H

#include <fmt/core.h>

#include <memory>
#include <mutex>

template<typename T>
class Singleton
{
public:
  static std::shared_ptr<T> getInstance()
  {
    static std::once_flag flag;
    std::call_once(flag, [&]() { m_instance = std::shared_ptr<T>(new T); });
    return m_instance;
  }

  ~Singleton() { fmt::println("Singleton destroyed"); }

protected:
  Singleton() = default;
  Singleton(const Singleton<T>&) = delete;
  Singleton& operator=(const Singleton<T>&) = delete;

  static std::shared_ptr<T> m_instance;
};

template<typename T>
std::shared_ptr<T> Singleton<T>::m_instance = nullptr;

#endif // SINGLETON_H