/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  UTILS - ReturnIfBad, Semaphore
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#ifndef HAM_UTILS_HPP
#define HAM_UTILS_HPP

#include "data.hpp" 
#include "w_pqueue.hpp" 
#include "logger.hpp"       // logger
#include "singleton.hpp"    // for logger

using namespace std;

namespace ham
{

class Semaphore
{
public:
    Semaphore(int count = 0): m_count(count) {}
    ~Semaphore() = default;

    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;

    void SetAll(int count)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_count = count;
        m_cv.notify_all();
    }

    void SetOne()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        ++m_count;
        m_cv.notify_one();
    }

    void SemWait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this]() { return m_count > 0; });

        --m_count;
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    int m_count;
};


inline void packBits(uint32_t& buffer, int& bitsInBuffer, int value, int bits)
{
    buffer |= (static_cast<uint32_t>(value) << bitsInBuffer);
    bitsInBuffer += bits;
}


#define RETURN_IF_BAD(IS_BAD, ERR_MSG)                   \
if (IS_BAD)                                              \
{                                                        \
    Logger* log = Singleton<Logger>::GetInstance();      \
    log->Log(LogLevel::ERROR, ERR_MSG, __LINE__, __FILE__); \
	return ;                                             \
};

}; //ham

#endif // HAM_UTILS_HPP

