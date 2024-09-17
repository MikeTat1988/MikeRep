/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  SINGLETON
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#ifndef HAM_SINGLETON_HPP
#define HAM_SINGLETON_HPP

#include <iostream>
#include <mutex>
#include <atomic>
#include <memory>   //unique_ptr

namespace ham
{

template<typename T>
class Singleton
{
public:

    Singleton() = delete;
    ~Singleton() = delete;
    Singleton(const Singleton&) = delete;
    const Singleton& operator=(const Singleton&) = delete;

    static T* GetInstance();

private:

    static std::atomic<T*> s_instance;
    static std::mutex s_mutex;

}; // Singleton

template<typename T>
std::mutex Singleton<T>::s_mutex;

template<typename T>
std::atomic<T*> Singleton<T>::s_instance = nullptr;

template<typename T>
T* Singleton<T>::GetInstance()
{
    std::atomic<T*> tmp = s_instance.load();

    std::atomic_thread_fence(std::memory_order_acquire);

    if (tmp == nullptr)
    {
        std::lock_guard<std::mutex> lock(s_mutex);

        tmp = s_instance.load();

        if (tmp == nullptr)
        {
            tmp = new T;

            // create unique ptr for auto destraction- reference counter
            static std::unique_ptr<T> s_autoDestarctor(tmp);

            std::atomic_thread_fence(std::memory_order_release);

            s_instance = tmp.load();
        }
    }

    return tmp;
}

} //ham

#endif // HAM_SINGLETON_HPP