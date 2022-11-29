#pragma once

class NonCopyable
{
public:
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
protected:
  NonCopyable() = default;
  ~NonCopyable() = default;
};

//Thread-Safe and fast Singleton Implementation
//Usage: class Foo : public GeneralPattern::Singleton<Foo>
template<typename T>
class Singleton
{
public:
  static T& GetInstance()
  {
    static T instance;
    return instance;
  }

  Singleton(const Singleton& s) = delete;
  void operator=(const Singleton& s) = delete;
protected:
  Singleton() {}
};