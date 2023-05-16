/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  THREADPOOL HPP
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/
#ifndef HAM_THREADPOOL_HPP
#define HAM_THREADPOOL_HPP

#include <iostream>
#include <chrono>
#include <mutex>                //mutex
#include <condition_variable>   //condition_variable
#include <vector>               // vector
#include <queue>                // queue
#include <thread>               // thread
#include <functional>           // function

#include "itask.hpp"
#include "w_pqueue.hpp"
#include "logger.hpp"
#include "singleton.hpp"
#include "utils.hpp"

namespace ham
{

class ThreadPool
{
private:
    static const int FACTOR = 2;

public:
    // place here note for user about hardware_concurrency
    ThreadPool(size_t numOfThreads_ = DefaultThreadNum()); 
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void PushTask(std::shared_ptr<ITask> task_);
    void SetSize(size_t newNumOfThreads_);

    void Stop(std::chrono::milliseconds& timeOut); // to indicate the reason to stop
    void Stop();
    void Resume();
    void Pause();

private:

    size_t numOfActiveThreads_;

    std::vector<std::thread> m_threads;

    WPQueue<std::shared_ptr<ITask>, std::vector<std::shared_ptr<ITask>>,
        bool(*)(std::shared_ptr<ITask> task1, std::shared_ptr<ITask> task2) > m_tasks; 

    void ThreadLoop() noexcept;

    static size_t DefaultThreadNum();

    static bool CompareFunc(std::shared_ptr<ITask> task1, std::shared_ptr<ITask> task2);

    Logger* m_log;

    class PauseThread;
    class StopException;
    class StopThread;

    static Semaphore s_sem;

}; // ThreadPool

class ThreadPool::PauseThread: public ITask
{
public:
    explicit PauseThread() : ITask(Priority::ADMIN) {}
    ~PauseThread() = default;

private:

    virtual void Execute();

};//class Pause

inline void ThreadPool::PauseThread::Execute()
{
    s_sem.SemWait();
}


class ThreadPool::StopException: public std::exception
{
public:
    StopException(const std::string& s_ = "StopTask"): message_(s_) {}
    const char* what() const noexcept override { return message_.c_str(); }
private:
    std::string message_;
};

class ThreadPool::StopThread: public ITask
{
public:
    explicit StopThread() : ITask(Priority::ADMIN) {}

    ~StopThread() = default;

private:

    virtual void Execute();

};//class StopThread

inline void ThreadPool::StopThread::Execute()
{
    throw StopException();
}

}// hrd27

#endif // HAM_THREADPOOL_HPP
