/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  THREADPOOL CPP
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#include <cassert> // assert
#include <sstream> // std::stringstream

#include "threadpool.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ham
{
Semaphore ThreadPool::s_sem = Semaphore();

ThreadPool::ThreadPool(size_t numOfThreads_)
    :numOfActiveThreads_(numOfThreads_),
    m_threads(numOfThreads_),
    m_tasks(CompareFunc),
    m_log(Singleton<Logger>::GetInstance())
{
    m_log->Log(LogLevel::DEBUG,"ThreadPool::ThreadPool(" + std::to_string(numOfThreads_) + ")", __LINE__, __FILE__);

    for (size_t i = 0; i < m_threads.size(); i++)
    {
        m_threads[i] = std::thread([this]() {ThreadLoop();});
    }

}

ThreadPool::~ThreadPool()
{
    m_log->Log(LogLevel::DEBUG,"ThreadPool::~ThreadPool()", __LINE__, __FILE__);

    if (m_threads.size())
    {
        try
        {
            Stop();
        }
        catch (const std::exception& e)
        {
            m_log->Log(LogLevel::INFO,e.what(), __LINE__, __FILE__);
        }
    }
}

void ThreadPool::PushTask(std::shared_ptr<ITask> task_)
{
    m_log->Log(LogLevel::DEBUG,"ThreadPool::PushTask", __LINE__, __FILE__);
    m_tasks.Push(task_);
}

void ThreadPool::SetSize(size_t newNumOfThreads_)
{
    m_log->Log(LogLevel::DEBUG,"ThreadPool::SetSize(" + std::to_string(newNumOfThreads_) + ")", __LINE__, __FILE__);

    size_t numAllThreads = m_threads.size();

    size_t numThreadsToPause = (numOfActiveThreads_ > newNumOfThreads_) ?
        (numOfActiveThreads_ - newNumOfThreads_) : 0;

    size_t numThreadsToAdd = (newNumOfThreads_ > numAllThreads) ?
        (newNumOfThreads_ - numAllThreads) : 0;

    size_t numThreadsToResume = ((newNumOfThreads_ - numThreadsToAdd) > numOfActiveThreads_) ?
        ((newNumOfThreads_ - numThreadsToAdd) - numOfActiveThreads_) : 0;


    for (size_t i = 0; i < numThreadsToResume; ++i)
    {
        s_sem.SetOne();
        ++numOfActiveThreads_;
    }

    for (size_t i = 0; i < numThreadsToPause; ++i)
    {
        m_tasks.Push(std::make_shared<PauseThread>());
        --numOfActiveThreads_;
    }

    for (size_t i = 0; i < numThreadsToAdd; ++i)
    {
        m_threads.emplace_back(std::thread([this]() { ThreadLoop(); }));
        ++numOfActiveThreads_;
    }

    assert(numOfActiveThreads_ == newNumOfThreads_);

}


void ThreadPool::ThreadLoop() noexcept
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    m_log->Log(LogLevel::DEBUG,"ThreadPool::ThreadLoop Start, id ", __LINE__, __FILE__);

    bool shouldStop = false;

    while (!shouldStop)
    {
        try
        {
            std::shared_ptr<ITask> task;
            m_tasks.Pop(task);
            task->Execute();
        }
        catch (const StopException& e)
        {
            shouldStop = true;
            m_log->Log(LogLevel::DEBUG,e.what(), __LINE__, __FILE__);
        }
        catch (const std::exception& e)
        {
            m_log->Log(LogLevel::DEBUG,e.what(), __LINE__, __FILE__);
        }
    }

    m_log->Log(LogLevel::DEBUG,"ThreadPool::ThreadLoop End, id ", __LINE__, __FILE__);
}

void ThreadPool::Stop(std::chrono::milliseconds& timeOut_)
{
    std::this_thread::sleep_for(timeOut_);
    Stop();
}

void ThreadPool::Stop()
{
    m_log->Log(LogLevel::DEBUG,"ThreadPool::Stop()", __LINE__, __FILE__);

    if (0 == m_threads.size())
    {
        m_log->Log(LogLevel::ERROR,"multiples Stop() calls", __LINE__, __FILE__);
        return;
    }

    // resume all threads
    Resume();

    // stop all threads
    for (size_t i = 0; i < m_threads.size(); ++i)
    {
        m_tasks.Push(std::make_shared<StopThread>());
    }

    numOfActiveThreads_ = 0;

    // join all threads
    for (auto& th : m_threads)
    {
        std::stringstream ss;
        ss << th.get_id();
        m_log->Log(LogLevel::DEBUG,"ThreadPool::Stop() - join, id " + ss.str(), __LINE__, __FILE__);
        th.join();
    }

    m_log->Log(LogLevel::DEBUG,"ThreadPool::Stop() - all joined", __LINE__, __FILE__);

    m_threads.clear(); // size = 0
}

void ThreadPool::Resume()
{
    m_log->Log(LogLevel::DEBUG,"ThreadPool::Resume()", __LINE__, __FILE__);

    // resume all threads
    size_t numPaused = m_threads.size() - numOfActiveThreads_;
    s_sem.SetAll(numPaused);
    numOfActiveThreads_ = m_threads.size();
}

void ThreadPool::Pause()
{
    m_log->Log(LogLevel::DEBUG,"ThreadPool::Pause()", __LINE__, __FILE__);

    // pause all active threads
    for (size_t i = 0; i < numOfActiveThreads_; ++i)
    {
        m_tasks.Push(std::make_shared<PauseThread>());
        --numOfActiveThreads_;
    }
}



size_t ThreadPool::DefaultThreadNum()
{
    size_t ret = std::thread::hardware_concurrency();

    if (0 == ret)
    {
        return 4;
    }

    return ret * FACTOR;
}

bool ThreadPool::CompareFunc(std::shared_ptr<ITask> task1_, std::shared_ptr<ITask> task2_)
{
    return *task1_ < *task2_;
}



} // namespace hrd27
