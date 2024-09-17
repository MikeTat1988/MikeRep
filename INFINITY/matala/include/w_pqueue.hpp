/******************************************************************************
  Project:      HOME_ASSIGNMENT 
  Description:  WAITABLE PRIORITY QUEUE
  Author:       Michael
  Date:         05/12/23
  Version:      1.0
 ******************************************************************************/

#ifndef HAM_WPQUEUE_HPP
#define HAM_WPQUEUE_HPP

#include <queue>                //priority_queue
#include <mutex>                //mutex
#include <condition_variable>   //condition_variable
#include <chrono>               //chrono::microseconds
#include <vector>
#include <iostream>

namespace ham
{

template<
    typename T,
    typename Container = std::vector<T>,
    typename Compare = std::less<typename Container::value_type>>
//T need to have copy constructor and assingment operator
class WPQueue
{
public:
    WPQueue(const Compare& compare_ = Compare());
    ~WPQueue() = default;

    WPQueue(const WPQueue&) = delete;
    WPQueue& operator=(const WPQueue&) = delete;

    void Push(const T& data_);
    void Pop(T& outParam_);    //if Pop to an empty Q: block
    bool Pop(T& outParam_, const std::chrono::milliseconds& timeOut);

    size_t Size() const;
    bool IsEmpty() const;

private:
    std::priority_queue<T, Container, Compare> m_q;
    std::condition_variable m_conVar;
    mutable std::mutex m_mutex;

};//WpriorityQ

template< typename T, typename Container, typename Compare>
WPQueue<T, Container, Compare>::WPQueue(const Compare& compare_): m_q(compare_, Container())
{}
    
template< typename T, typename Container, typename Compare>
void WPQueue<T, Container, Compare>::Push(const T& data_)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    m_q.push(data_);

    m_conVar.notify_one();
}


template< typename T, typename Container, typename Compare>
void WPQueue<T, Container, Compare>::Pop(T& outParam_)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    m_conVar.wait(lock, [this] { return !m_q.empty(); });

    outParam_ = m_q.top();

    m_q.pop();
}

template< typename T, typename Container, typename Compare>
bool WPQueue<T, Container, Compare>::Pop(T& outParam_,
    const std::chrono::milliseconds& timeOut)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    bool result = m_conVar.wait_for(lock, timeOut, [this] { return !m_q.empty(); });
    
    if (result)
    {
        outParam_ = m_q.top();
        m_q.pop();
    }

    return result;
}

template< typename T, typename Container, typename Compare>
size_t WPQueue<T, Container, Compare>::Size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_q.size();
}

template< typename T, typename Container, typename Compare>
bool WPQueue<T, Container, Compare>::IsEmpty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_q.empty();
}

} // end namespace ham

#endif // HAM_WPQUEUE_HPP